
Your First Step
>>>>>>>>>>>>>>>>>>>>>>>
Installaion of the CUDD Package (version 2.4.2 or higher).  Refer to it documentation or follow the following steps to compile it.



This directory contains a set of packages that allow you to build a toy
application based on the CUDD package.

The CUDD package is a package written in C for the manipulation of
decision diagrams.  It supports binary decision diagrams (BDDs),
algebraic decision diagrams (ADDs), and Zero-Suppressed BDDs (ZDDs).

The toy application provided in this kit is called nanotrav and is a
simple-minded FSM traversal program. (See the README file and the man
page nanotrav.1 in the nanotrav directory for the details.)  It is
included so that you can run a sanity check on your installation.

INSTALLATION
Before you build the libraries and programs, you need to check the
Makefile in the top directory.  Go through the definitions contained in the
configuration section, and select the desired compiler and compilation
flags.  Instructions are provided in the comments of the Makefile.

You can always specify the options on the command line.  For instance,
on some machines you can build a "fast" version of the program by typing:

    make DDDEBUG= MTRDEBUG= ICFLAGS=-O2

    The Makefile supports several targets:

    make:
	Creates a "plain" version of the program.

    make testdddmp:
	Builds a test program (testdddmp) for BDD loading from and
	storing to disk. See file README.test in the dddmp directory for
	how to run the program.

    make testobj:
	Builds a test program for the C++ interface. Requires a C++
	compiler. To run the program, run obj/testobj.

    make testcudd:
	Builds a test program for CUDD. To run the program, go to the
	cudd directory and type "./testcudd -p 2 r7x8.1.mat". The result
	can be compared to r7x7.1.out.

    make testmtr:
        Builds a test program for the mtr package.  To run the program,
	go to the mtr directory and type "./testmtr -p 1 test.groups".
    
    make clean:
	Cleans directories, but leaves libraries and programs.

    make distclean:
	Cleans thoroughly, returning the directories to their pristine
	state.

The following targets are more or less obsolete and may disappear or
change in the future.

    make check_leaks: 
	Creates a version of the program with the mnemosyne library
	linked to it. It also builds the mnemalyse program, which
	helps in finding memory leaks. This target does not work on the
	IBM RS6000. The makefile also supports purify. To use purify,
	set the PURE variable in the Makefile, and use the standard
	target.

    make optimize_dec:
	Builds a version of the program using the u-code compiler
	available on DEC machines (DECstations and Alphas). The newer
	native compiler on the Alphas does not use u-code, though.
	Therefore the standard target should be used with it.

    make lint:
	Runs lint on all subdirectories except mnemosyne. Creates lint
	libraries for all object libraries.

    make tags:
	Builds ctags-style tag files for all subdirectories except
	mnemosyne.

    make all:
	Makes all of the above, except check_leaks, which is
	incompatible with a plain "make."

All targets, except clean and distclean, will create the include
directory if it does not already exist.

The Makefile does not compile the SIS interface (cuddBddPort.c and
cuddPwPt.c found in subdirectory sis).  To compile the interface, you
also need array.h and var_set.h, which are not part of this
distribution, but come with SIS.  Detailed instructions on how to
integrate the CUDD package in SIS can be found in the documentation
(cudd/doc).

PLATFORMS

This kit has been successfully built on the following configurations:
    PC (ia32 and ia64) running Linux RedHat with gcc
    PC (ia32 and ia64) running Linux RedHat with g++
    PC (ia32) running Linux RedREADME file for the DDDMP-2.0 package
Revision: Version 2.0.2, February 01, 2004



Step Two
>>>>>>>>>>>>>>>>>
Insall Limxml2 (version 2.7.2 or higher);
It can be downloaded from http://www.xmlsoft.org/
follow the instruction and build the libxml.
You will find the installation of RelMDD easier or linking this library with RelMDD if you install Limxml2 in 
the /usr  directory of linux system.





Step Three
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

MAKE RelMDD
==========

Before you build the libraries and programs, you need to check the
Makefile in the top directory.
Go through the definitions contained in the configuration section, and
select the desired compiler and compilation flags.
Instructions are provided in the comments of the Makefile.
Then run "make".
This should produce the RelMDDlib.a library.


Step Four
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

1) Now to use the library you have to link RelMDDlib.a to your object files. 
2) ADD RelMDD.h and strutures.h to your headers or source code.
3) In order to avoid comficticts. go to the CUDD package and look for 
cuddMatMult.c file and delete it from folder. otherwise you might have problems with the use of compostion/ multiplication

you can also you our version of CUDD accompaning the RelMDD package. 




DOCUMENTATION
=============

Directory RelMDD/doc contains HTML documentation for the package.
The recommended starting point is package.html.
Documentation in both postscript format and plain text format is also
provided.

PLATFORMS

This kit has been successfully built on the following configurations:
    PC (ia32 and ia64) running Linux RedHat with gcc
    PC (ia32 and ia64) running Linux RedHat with g++
    PC (ia32) running Linux RedREADME file 



The documentation above is taken from CUDD so credit goes to CUDD (Fabio Somenzi)


