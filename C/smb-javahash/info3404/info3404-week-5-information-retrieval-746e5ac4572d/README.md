# Week 5 PASTA Homework - Information Retrieval
This weeks homework revolves around informational retrieval using the Vector Space model. The lecture slides provide a good outline of this.

You are implementing a IR Vector Engine. This will allow you to input several documents, and then perform a query on these documents to find those that are the most similar.

As usual, this week there are 3 different levels you can choose to do, to get 2, 3 or 4 marks respectively.

## Easy
You need to implement the following methods in the classes:

* Engine: calculateRecall, calculatePrecision, calculateFallout
* IRBooleanEngine: execute, constructor
* DTMatrix: getInverseDocumentFrequency, addDocument
* SimpleTokenizer: parse

The sample tests for this level are:

* DTMatrixTest
* IRBooleanEngineTest

As part of this, you will need to ignore any stop words that may be in the documents or queries. The list of stop words to use can be found in the core.Constants file

Feel free to add as many extra methods as you feel you may need to complete this task, however it is possible to achieve this task without needing to do that.

## Medium
Here we ask you to expand upon the IR used in easy, and modify it to use the cosine similarity measure to calculate the relevance values of each document

You will need to implement [Easy], as well as:

* IRBooleanWithCosineEngine: calculateNormalisationVector, execute

The sample tests for this level are:

* DTMatrixTest
* IRBooleanWithCosineEngineTest

## Hard
While the current system is great, when we try to use words like "fishing" and "fish" they're treated as separate which is less than ideal because they're essentially referring to the same concept. Modify IRBooleanWithCosineEngine (or whatever other classes you see fit), to stem the words used in the document and the query. There is a Stemmer class provided (PorterStemmer). You will need to build off [Medium] to achieve this.

The sample tests for this level are:

* DTMatrixTest
* IRBooleanWithCosineEngineStemmerTest
* StemmingWordsTest