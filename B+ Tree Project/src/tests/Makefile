JAR=btreelib.jar

#this is the name of the given project folder
ASSIGNMENT=btree_project_spring_25

#change the ASSIGN path to the path where you have downloaded
ASSIGN=/home/yke3135/btree_project_spring_25

# Do not change the following paths if you are using omega.uta.edu (Spring 2025)
LIBPATH = $(ASSIGN)/$(ASSIGNMENT)/lib/$(JAR)
CLASSPATH = $(LIBPATH):$(ASSIGN)/$(ASSIGNMENT)/src
JAVAC = javac -classpath $(CLASSPATH)
JAVA  = java  -classpath $(CLASSPATH)

BTTest:BTTest.java
	$(JAVAC) BTTest.java TestDriver.java

bttest: BTTest
	$(JAVA) tests.BTTest

clean:
	\rm -f *.class *~ \#* core
