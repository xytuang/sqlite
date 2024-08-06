# Introduction

This document designs a simplified sqlite clone. The goal of this project is not to build a 1-to-1 clone of sqlite, but rather to build a rough skeleton where the core functionality of sqlite is implemented.

# User Requirements

User can store large amounts of data with support for various data types.

User can retrieve data using complex queries in an efficient manner.

User can insert, update, delete, and manage data.

User is guranteed accuracy and consistency of data.

# High level design

![Sqlite Internals](images/sqlite-internals "Sqlite Internals")

# Detailed design

Frontend (tokenizer, parser and code generator):Turns a SQL query into sqlite virtual machine bytecode.

Backend (virtual machine, B-tree, pager and OS inteface)

The virtual machine takes bytecode from frontend and performs operations on tables/B-trees based on the given bytecode instructions.
 
The B-tree is the data structure for a table. A B-tree has many nodes, each of which is one page in size. B-tree can retrieve a page from disk or save it to disk by issuing commands to pager.

The pager receives commands to read or write pages of data. It is responsible for reading/writing at appropriate offsets in the database file. It keeps a cache of recently accessed pages in memory and writes them back to disk when needed.

The OS interface differs depending on the operating system being targeted. It is not going to be handled in this project.

# Limitations and future work

Concurrency control has not been implemented.

Implement caching strategies.

# References

https://www.sqlite.org/arch.html
