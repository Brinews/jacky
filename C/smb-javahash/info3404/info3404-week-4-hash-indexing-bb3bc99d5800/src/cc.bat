javac -d ./class core/Bucket.java core/DataBaseConstants.java core/HashTable.java core/Tuple.java

javac  -classpath ./lib/junit.jar;./class -d ./class statichashing/StaticHashTable.java statichashing/StaticHashTableTest.java

javac  -classpath ./lib/junit.jar;./class -d ./class extendiblehashing/ExtendibleHashTable.java extendiblehashing/ExtendibleHashTableTest.java

javac  -classpath ./lib/junit.jar;./class -d ./class linearhashing/LinearHashTable.java linearhashing/LinearHashTableTest.java

