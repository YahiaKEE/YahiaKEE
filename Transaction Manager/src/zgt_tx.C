/***************** Transaction class **********************/
/*** Implements methods that handle Begin, Read, Write, ***/
/*** Abort, Commit operations of transactions. These    ***/
/*** methods are passed as parameters to threads        ***/
/*** spawned by Transaction manager class.              ***/
/**********************************************************/

/* Spring 2025: CSE 4331/5331 Project 2 : Tx Manager */

/* Required header files */

//FINAL V1 3/18/25 640pm

#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include "zgt_def.h"
#include "zgt_tm.h"
#include "zgt_extern.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

extern void *start_operation(long, long);  // start an op with mutex lock and cond wait
extern void *finish_operation(long);       // finish an op with mutex unlock and con signal

extern void *do_commit_abort_operation(long, char);  // commit/abort based on char value
extern void *process_read_write_operation(long, long, int, char);

extern zgt_tm *ZGT_Sh;  // Transaction manager object

/* Transaction class constructor */
/* Initializes transaction id and status and thread id */
/* Input: Transaction id, status, thread id */

zgt_tx::zgt_tx( long tid, char Txstatus,char type, pthread_t thrid)
{
    this->lockmode = (char)' '; // default
    this->Txtype = type; // R = read only, W=Read/Write
    this->sgno =1;
    this->tid = tid;
    this->obno = -1; // set it to a invalid value
    this->status = Txstatus;
    this->pid = thrid;
    this->head = NULL;
    this->nextr = NULL;
    this->semno = -1; // init to an invalid sem value
}

/* Method used to obtain reference to a transaction node */
/* Inputs the transaction id. Makes a linear scan over the */
/* linked list of transaction nodes and returns the reference */
/* of the required node if found. Otherwise returns NULL */

zgt_tx* get_tx(long tid1)
{
    zgt_tx *txptr, *lastr1;
    if(ZGT_Sh->lastr != NULL) // If the list is not empty
    {
        lastr1 = ZGT_Sh->lastr; // Initialize lastr1 to first node's ptr
        for (txptr = lastr1; (txptr != NULL); txptr = txptr->nextr)
        {
            if (txptr->tid == tid1) // if required id is found
               return txptr;
        }
        
        return (NULL); // If not found in list return NULL
    }

    return(NULL); // If list is empty return NULL
}

/* Method that handles "BeginTx tid" in test file */
/* Inputs a pointer to transaction id, obj pair as a struct. Creates a new  */
/* transaction node, initializes its data members and */
/* adds it to transaction list. */

void *begintx(void *arg)
{
    //intialise a transaction object. Make sure it is 
  //done after acquiring the semaphore for the tm and making sure that 
  //the operation can proceed using the condition variable. When creating
  //the tx object, set the tx to TR_ACTIVE and obno to -1; there is no 
  //semno as yet as none is waiting on this tx.

    struct param *node = (struct param*)arg; // get tid and count
    start_operation(node->tid, node->count);
      zgt_tx *tx = new zgt_tx(node->tid,TR_ACTIVE, node->Txtype, pthread_self()); // Create new tx node

      //Writes the Txtype to the file

      zgt_p(0);				// Lock Tx manager; Add node to transaction list
  
      tx->nextr = ZGT_Sh->lastr;
      ZGT_Sh->lastr = tx;   
      zgt_v(0); 			// Release tx manager 
    fprintf(ZGT_Sh->logfile, "T%d\t%c \tBeginTx\n", node->tid, node->Txtype);	// Write log record and close
      fflush(ZGT_Sh->logfile);
    finish_operation(node->tid);
    pthread_exit(NULL);				// thread exit
}

/* Method to handle Readtx action in test file.   */
/* Inputs a pointer to structure that contains    */
/* tx id and object no to read. Reads the object */
/* if the object is not yet present in hash table */
/* or same tx holds a lock on it. Otherwise waits */
/* until the lock is released */

void *readtx(void *arg) {
    // Extract the transaction parameters from the arg struct
    struct param *node = (struct param*)arg;
    start_operation(node->tid, node->count); // start operation

    // Get the transaction object corresponding to the given transaction ID
    zgt_tx *currentTx = get_tx(node->tid);
    if (!currentTx) { 
        // If the transaction doesnt exist, finish operation and exit thread
        finish_operation(node->tid);
        pthread_exit(NULL);
    }

    // Lock transaction manager and check if the object is already locked by another transaction
    zgt_p(0);
    zgt_hlink *lockedObject = ZGT_Ht->find(1, node->obno);
    zgt_v(0); // Unlock transaction manager after checking

    // Log transaction read attempt
    printf("T%d\tReadTx\t%d\n", node->tid, node->obno);
    fflush(stdout);
    
    // If logging enabled, flush
    if (ZGT_Sh->logfile) {
        fflush(ZGT_Sh->logfile);
    }

    // Using do-while for transaction handling (3/15/25)
    do {
        if (!lockedObject) {  
            // Case 1: If object is not locked, get a shared lock and proceed
            zgt_p(0); // Lock transaction manager to modify lock table
            ZGT_Ht->add(currentTx, 1, node->obno, 'S'); // Add shared lock to lock table
            zgt_v(0); // Unlock transaction manager
            currentTx->perform_read_write_operation(node->tid, node->obno, 'S'); // Perform read 
            break; // Exit block
        }

        if (lockedObject->tid == node->tid) {  
            // Case 2: If current transaction already holds lock, proceed w read
            currentTx->perform_read_write_operation(node->tid, node->obno, 'S');
            break; // Exit block
        }

        // Case 3: Check if both current transaction and the lock holder are read only transactions
        zgt_tx *holdingTx = get_tx(lockedObject->tid); // Get transaction holding lock
        if (currentTx->Txtype == 'R' && holdingTx->Txtype == 'R' && 
            zgt_nwait(lockedObject->tid) <= 0) { 
            // If both transactions are read only and no one is waiting on the lock, proceed w reading
            currentTx->perform_read_write_operation(node->tid, node->obno, 'S');
            break; // Exit block
        }

        // Case 4: current transaction must wait for lock
        currentTx->status = TR_WAIT; // Mark transaction as waiting
        currentTx->obno = node->obno; // Store object number for reference
        currentTx->lockmode = 'S'; // Set lock mode to shared

        // transaction -> lock-holding transaction’s semaphore
        currentTx->setTx_semno(lockedObject->tid, lockedObject->tid);
        zgt_p(lockedObject->tid); // Block transaction until it acquires lock

        // transaction is unblocked -> so mark it as active and proceed w read
        currentTx->status = TR_ACTIVE; // Mark transaction as active again
        currentTx->obno = -1; // Reset object# since lock is now held
        currentTx->lockmode = 'S'; // Maintain shared lock mode
        currentTx->perform_read_write_operation(node->tid, node->obno, 'S'); // Perform read

        // Release lock for next waiting transaction
        zgt_v(lockedObject->tid);

    } while (0); // Exit block

    // Complete operation and exit the thread
    finish_operation(node->tid);
    pthread_exit(NULL);
}



void *writetx(void *arg) {
    // Extract the transaction parameters from the arg struct
    struct param *node = (struct param*)arg;
    start_operation(node->tid, node->count); //start operation

     // Get the transaction object corresponding to the given transaction ID
    zgt_tx *currentTx = get_tx(node->tid);
    if (!currentTx) { 
        // If the transaction doesnt exist, finish operation and exit
        finish_operation(node->tid);
        pthread_exit(NULL);
    }

    // Lock transaction manager and check if object is already locked by another transaction
    zgt_p(0); // Get lock to check hash table
    zgt_hlink *lockedObject = ZGT_Ht->find(1, node->obno); // Check lock table for object
    zgt_v(0); // Release lock after checking

    // Log write attempt for debugging and tracking
    printf("T%d\tWriteTx\t%d\n", node->tid, node->obno);
    fflush(stdout);
    
    // If logging enabled, flush
    if (ZGT_Sh->logfile) {
        fflush(ZGT_Sh->logfile);
    }

    // Use do while to structure execution 
    do {
        if (!lockedObject) {  
            // Case 1: If object is not locked, get exclusive lock and proceed
            zgt_p(0); // Lock transaction manager before modifying lock table
            ZGT_Ht->add(currentTx, 1, node->obno, 'X'); // Add exclusive lock 
            zgt_v(0); // Unlock transaction manager after updating table
            
            // Perform write
            currentTx->perform_read_write_operation(node->tid, node->obno, 'X');
            break; // Exit block
        }

        if (lockedObject->tid == node->tid) {  
            // Case 2: If current transaction already owns lock, proceed w writing
            currentTx->perform_read_write_operation(node->tid, node->obno, 'X');
            break; // Exit block
        }

        // Case 3: object is locked by another transaction, so current transaction must wait
        currentTx->status = TR_WAIT; // Change status to waiting
        currentTx->obno = node->obno; // Store object#
        currentTx->lockmode = 'X'; //  exclusive lock request

        // Put transaction into wait queue of the transaction currently holding lock
        currentTx->setTx_semno(lockedObject->tid, lockedObject->tid);
        zgt_p(lockedObject->tid); // Block transaction until it acquires lock

        // Once transaction unblocked, mark as active & proceed w writing
        currentTx->status = TR_ACTIVE; // Change status to active
        currentTx->obno = -1; // Reset object# bc lock is acquired
        currentTx->lockmode = 'X'; // Maintain exclusive lock mode

        // Perform write operation now bcz lock is acquired
        currentTx->perform_read_write_operation(node->tid, node->obno, 'X');

        // Release lock so that other transactions waiting on object can proceed
        zgt_v(lockedObject->tid);
    } while (0); // Exit block

    // Complete operation and exit thread
    finish_operation(node->tid);
    pthread_exit(NULL);
}


void *aborttx(void *arg) {
    // Extract the transaction parameters from the arg struct
    struct param *node = (struct param*)arg;
    
    // Start of operation using mutex locks and conditional waits
    start_operation(node->tid, node->count);

    // Get transaction object for given transaction ID
    zgt_tx *currentTx = get_tx(node->tid);

    // Log and print abort transaction operation
    printf("T%d\tAbortTx\n", node->tid);
    fflush(stdout);

    if (ZGT_Sh->logfile) { 
        // If logging is enabled, write abort transaction to log file
        fprintf(ZGT_Sh->logfile, "T%d\tAbortTx\n", node->tid);
        fflush(ZGT_Sh->logfile);
    }

    // Check if transaction exists
    if (currentTx) { 
        // Free all locks held by transaction before removing it
        currentTx->free_locks();

        // Remove transaction from transaction manager table
        currentTx->remove_tx();
    } else { 
        // If transaction does not exist, print error mesg
        printf("ERROR: Attempted to remove non-existent Tx %d\n", node->tid);
        fflush(stdout);

        if (ZGT_Sh->logfile) { 
            // Log error in the log file if enabled
            fprintf(ZGT_Sh->logfile, "ERROR: Attempted to remove non-existent Tx %d\n", node->tid);
            fflush(ZGT_Sh->logfile);
        }
    }

    // Finish transaction operation and release synchronization locks
    finish_operation(node->tid);
    
    // Terminate thread execution after completing abort
    pthread_exit(NULL);
}

/* Method to commit a transaction. The commit operation frees */
/* all the locks/objects held by the committing Tx and */
/* releases all Txs waiting on the committing Tx. */
/* Finally, the commiting Tx object is removed from the TM table */

void *committx(void *arg) {
    // Extract transaction parameters from the argument str
    struct param *node = (struct param*)arg;
    
    // Start of commit operation- using mutex locks and conditional waits
    start_operation(node->tid, node->count);

    // Get transaction object associated with given transaction ID
    zgt_tx *currentTx = get_tx(node->tid);

    // Log and print commit transaction operation
    printf("T%d\tCommitTx\n", node->tid);
    fflush(stdout); // makes sure immediate output to console

    if (ZGT_Sh->logfile) { 
        // If logging is enabled, write commit transaction event to log file
        fprintf(ZGT_Sh->logfile, "T%d\tCommitTx\n", node->tid);
        fflush(ZGT_Sh->logfile); // Make sure immediate write to log file
    }

    // Verify if transaction exists before proceeding w commit
    if (currentTx) { 
        // step 1: Release all locks held by transaction
        currentTx->free_locks();

        // step 2: Remove transaction from transaction manager table
        currentTx->remove_tx();
    } else { 
        // If transaction does not exist, print error msg
        printf("ERROR: Attempted to remove non-existent Tx %d\n", node->tid);
        fflush(stdout); // makes sure immediate error message output

        if (ZGT_Sh->logfile) { 
            // Log error in log file if logging is enabled
            fprintf(ZGT_Sh->logfile, "ERROR: Attempted to remove non-existent Tx %d\n", node->tid);
            fflush(ZGT_Sh->logfile); // makes sure immediate write to log file
        }
    }

    // Finish commit operation and release synchronization locks
    finish_operation(node->tid);
    
    // Finish thread execution after completing commit process
    pthread_exit(NULL);
}



//suggestion as they are very similar

// called from commit/abort with appropriate parameter to do the actual
// operation. Make sure you give error messages if you are trying to
// commit/abort a non-existent tx

void *do_commit_abort_operation(long t, char status)
{
    //We do not use this - 3/13/25
}


int zgt_tx::remove_tx ()
{
    //remove the transaction from the TM

    zgt_tx *txptr, *lastr1;
    lastr1 = ZGT_Sh->lastr;
    for(txptr = ZGT_Sh->lastr; txptr != NULL; txptr = txptr->nextr) // scan through list
    {
        if (txptr->tid == this->tid)
        {
            // if correct node is found
            
            lastr1->nextr = txptr->nextr; // update nextr value; done
                                          // delete this;
            return(0);
        }
        else lastr1 = txptr->nextr; // else update prev value
        
    }
    
    fprintf(ZGT_Sh->logfile, "Trying to Remove a Tx:%d that does not exist\n", this->tid);
  fflush(ZGT_Sh->logfile);
  printf("Trying to Remove a Tx:%d that does not exist\n", this->tid);
  fflush(stdout);
  return(-1);
}

/* this method sets lock on objno1 with lockmode1 for a tx */

int zgt_tx::set_lock(long tid1, long sgno1, long obno1, int count, char lockmode1)
{
  //if the thread has to wait, block the thread on a semaphore from the
  //sempool in the transaction manager. Set the appropriate parameters in the
  //transaction list if waiting.
  //if successful  return(0); else -1

  //we do not use - 3/14/25
  
}

int zgt_tx::free_locks()
{

    // this part frees all locks owned by the transaction
  // that is, remove the objects from the hash table
  // and release all Tx's waiting on this Tx

    zgt_hlink* temp = head; // first obj of tx

    for(temp;temp != NULL;temp = temp->nextp) // SCAN Tx obj list
    {
        fprintf(ZGT_Sh->logfile, "%d : %d, ", temp->obno, ZGT_Sh->objarray[temp->obno]->value);
        fflush(ZGT_Sh->logfile);

        if (ZGT_Ht->remove(this,1,(long)temp->obno) == 1)
        {
            printf(":::ERROR:node with tid:%d and onjno:%d was not found for deleting", this->tid, temp->obno);		// Release from hash table
	        fflush(stdout);
        }
        else
        {
            #ifdef TX_DEBUG
                printf("\n:::Hash node with Tid:%d, obno:%d lockmode:%c removed\n",
                                temp->tid, temp->obno, temp->lockmode);
                fflush(stdout);
            #endif
        }
    }

    fprintf(ZGT_Sh->logfile, "\n");
  fflush(ZGT_Sh->logfile);

    return(0);
}		

// CURRENTLY Not USED
// USED to COMMIT
// remove the transaction and free all associate dobjects. For the time being
// this can be used for commit of the transaction.

int zgt_tx::end_tx()
{
    zgt_tx *linktx, *prevp;

    // USED to COMMIT 
  //remove the transaction and free all associate dobjects. For the time being 
  //this can be used for commit of the transaction.

    linktx = prevp = ZGT_Sh->lastr;

    while (linktx) {
        if (linktx->tid  == this->tid) break;
        prevp  = linktx;
        linktx = linktx->nextr;
    }
    if (linktx == NULL) {
        printf("\ncannot remove a Tx node; error\n");
        fflush(stdout);
        return (1);
    }
    if (linktx == ZGT_Sh->lastr) ZGT_Sh->lastr = linktx->nextr;
    else {
        prevp = ZGT_Sh->lastr;
        while (prevp->nextr != linktx) prevp = prevp->nextr;
        prevp->nextr = linktx->nextr;
    }
}

// currently not used
int zgt_tx::cleanup()
{
  return(0);
  
}

// routine to print the tx list
// TX_DEBUG should be defined in the Makefile to print
void zgt_tx::print_tm()
{
    zgt_tx *txptr;

    #ifdef TX_DEBUG
  printf("printing the tx  list \n");
  printf("Tid\tTxType\tThrid\t\tobjno\tlock\tstatus\tsemno\n");
  fflush(stdout);
#endif
  txptr=ZGT_Sh->lastr;
  while (txptr != NULL) {
#ifdef TX_DEBUG
    printf("%d\t%c\t%d\t%d\t%c\t%c\t%d\n", txptr->tid, txptr->Txtype, txptr->pid, txptr->obno, txptr->lockmode, txptr->status, txptr->semno);
    fflush(stdout);
#endif
    txptr = txptr->nextr;
  }
  fflush(stdout);
}

//need to be called for printing
void zgt_tx::print_wait()
{
  //route for printing for debugging
  
  printf("\n    SGNO        TxType       OBNO        TID        PID         SEMNO   L\n");
  printf("\n");
}


void zgt_tx::print_lock()
{
  //routine for printing for debugging
  
  printf("\n    SGNO        OBNO        TID        PID   L\n");
  printf("\n");
  
}

// routine to perform the actual read/write operation as described the project description
// based  on the lockmode

void zgt_tx::perform_read_write_operation(long tid, long obno, char lockmode) {
    // Get current value of object from transaction manager's object array
    int currentObjValue = ZGT_Sh->objarray[obno]->value;

    // Declare variables to store transaction type and lock type
    const char *operationType = nullptr;
    const char *lockType = nullptr;

    // Retrieve optime directly from transaction manager
    int operationTime = ZGT_Sh->optime[tid];  // directly using from header

    // Determine both operation type (ReadTx or WriteTx) and lock type (ReadLock or WriteLock)
    if (lockmode == 'S') {
        operationType = "ReadTx";  // Read operation type
        lockType = "ReadLock";     // Shared lock for reading
    } else {  
        operationType = "WriteTx"; // Write operation type
        lockType = "WriteLock";    // Exclusive lock for writing
    }

    // If operation is write transaction, update object value in transaction manager
    if (lockmode == 'X') {
        ZGT_Sh->objarray[obno]->value += 1; // Increment object's value
        currentObjValue = ZGT_Sh->objarray[obno]->value; // Update local copy
    }

    char transactionDetails[150]; // Buffer to store transaction message
    snprintf(transactionDetails, sizeof(transactionDetails), 
             "T%d\t%s\t%d:%d:%d\t%s\tGranted\n", 
             tid, operationType, obno, currentObjValue, operationTime, lockType);

    // Print transaction details to console
    printf("%s", transactionDetails);
    fflush(stdout); // makes sure immediate output

    // If logging enabled, write transaction details to log file
    if (ZGT_Sh->logfile) {
        fprintf(ZGT_Sh->logfile, "%s", transactionDetails);
        fflush(ZGT_Sh->logfile); // Ensure log file is updated immediately
    }
}


// routine that sets the semno in the Tx when another tx waits on it.
// the same number is the same as the tx number on which a Tx is waiting

int zgt_tx::setTx_semno(long tid, int semno)
{
    zgt_tx *txptr;

    txptr = get_tx(tid);
    if (txptr == NULL){
        printf("\n:::ERROR:Txid %d wants to wait on sem:%d of tid:%d which does not exist\n", this->tid, semno, tid);
        fflush(stdout);
        exit(1);
    }
    
    if ((txptr->semno == -1)|| (txptr->semno == semno)){  //just to be safe
        txptr->semno = semno;
        return(0);
    }
    else if (txptr->semno != semno){
        #ifdef TX_DEBUG
            printf(":::ERROR Trying to wait on sem:%d, but on Tx:%d\n", semno, txptr->tid);
            fflush(stdout);
        #endif
        exit(1);
    }
    
    return(0);
}


void *start_operation(long tid, long count)
{
    pthread_mutex_lock(&ZGT_Sh->mutexpool[tid]); // Lock mutex[t] to make other
    // threads of same transaction to wait

    while(ZGT_Sh->condset[tid] != count) // wait if condset[t] is != count
    {
      pthread_cond_wait(&ZGT_Sh->condpool[tid],&ZGT_Sh->mutexpool[tid]);
    }
}

// Otherside of teh start operation;
// signals the conditional broadcast

void *finish_operation(long tid)
{
    ZGT_Sh->condset[tid]--; // decr condset[tid] for allowing the next op
    pthread_cond_broadcast(&ZGT_Sh->condpool[tid]); // other waiting threads of same tx
    pthread_mutex_unlock(&ZGT_Sh->mutexpool[tid]);

}

