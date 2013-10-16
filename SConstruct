##########################################################################
#
#  Copyright (c) 2011-2012, John Haddon. All rights reserved.
#  Copyright (c) 2011-2013, Image Engine Design Inc. All rights reserved.
#  Copyright (c) 2013, Luke Goddard. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
#     * Neither the name of Image Engine Design nor the names of any
#       other contributors to this software may be used to endorse or
#       promote products derived from this software without specific prior
#       written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
##########################################################################
import os
import re
import sys
import glob
import shutil
import fnmatch
import py_compile
import subprocess

###############################################################################################
# Command line options
###############################################################################################

optionsFile = None

if "GANDER_OPTIONS_FILE" in os.environ :
	optionsFile = os.environ["GANDER_OPTIONS_FILE"]
	
if "OPTIONS" in ARGUMENTS :
	optionsFile = ARGUMENTS["OPTIONS"]
	
options = Variables( optionsFile, ARGUMENTS )

options.Add(
	"TestCase",
	"The name of the test suite or test case to run.",
	""
)

options.Add(
	"CXX",
	"The C++ compiler.",
	"g++",
)

options.Add(
	"CXXFLAGS",
	"The extra flags to pass to the C++ compiler during compilation.",
	[ "-fPIC", "-pipe", "-Wall", "-Werror", "-O2", "-DNDEBUG", "-DBOOST_DISABLE_ASSERTS" ]
)

options.Add(
        "LINKFLAGS",
        "The extra flags to pass to the C++ linker during compilation.",
        "",
)

options.Add(
	"BUILD_DIR",
	"The destination directory in which the build will be made.",
	"./build/Gander-${GANDER_MAJOR_VERSION}.${GANDER_MINOR_VERSION}.${GANDER_PATCH_VERSION}-${PLATFORM}",
)

options.Add(
	"BUILD_CACHEDIR",
	"Specify a directory for SCons to cache build results in. This allows the sharing of build results"
	"among multiple developers and can significantly reduce build times, particularly when switching"
	"between multiple compilers and build options.",
	""
)

options.Add(
	"INSTALL_DIR",
	"The destination directory for the installation.",
	"",
)

options.Add(
	"PACKAGE_FILE",
	"The file in which the final Gander file will be created by the package target.",
	"${INSTALL_DIR}.tar.gz",
)

options.Add( 
	BoolVariable( "BUILD_DEPENDENCIES", "Set this to build all the library dependencies Gander has.", False )
)

# variables related to building all the dependencies for Gander. these are mutually exclusive
# with the LOCATE_* below, which are about finding the dependencies in existing locations.
# use the BUILD_* options to make a completely standalone package and the other options to
# make a build to integrate into an existing setup where the dependencies have been installed
# somewhere centrally.

options.Add(
	"DEPENDENCIES_SRC_DIR",
	"The location of a directory holding dependencies.",
	"",
)

options.Add(
	BoolVariable( "BUILD_DEPENDENCY_EIGEN", "Set this to build eigen.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"EIGEN_SRC_DIR",
	"The location of the eigen source to be used if BUILD_DEPENDENCY_EIGEN is specified.",
	"$DEPENDENCIES_SRC_DIR/eigen-3.1.4",
)

options.Add(
	BoolVariable( "BUILD_DEPENDENCY_BOOST", "Set this to build boost.", "$BUILD_DEPENDENCIES" )
)

options.Add(
	"BOOST_SRC_DIR",
	"The location of the boost source to be used if BUILD_DEPENDENCY_BOOST is specified.",
	"$DEPENDENCIES_SRC_DIR/boost_1_53_0",
)

# variables to be used when making a build which will use dependencies previously
# installed in some central location. these are mutually exclusive with the BUILD_*
# variables above, which are all about building the dependencies and packaging them
# with Gander.

options.Add(
	"LOCATE_DEPENDENCY_CPPPATH",
	"Locations on which to search for include files "
	"for the dependencies. These are included with -I.",
	[],
)

options.Add(
	"LOCATE_DEPENDENCY_SYSTEMPATH",
	"Locations on which to search for include files "
	"for the dependencies. These are included with -isystem.",
	"",
)

options.Add(
	"LOCATE_DEPENDENCY_LIBPATH",
	"The locations on which to search for libraries for "
	"the dependencies.",
	"",
)

options.Add(
	"BOOST_LIB_SUFFIX",
	"The suffix used when locating the boost libraries.",
	"",
)

# general variables

options.Add(
	"ENV_VARS_TO_IMPORT",
	"By default SCons ignores the environment it is run in, to avoid it contaminating the "
	"build process. This can be problematic if some of the environment is critical for "
	"running the applications used during the build. This space separated list of environment "
	"variables is imported to help overcome these problems.",
	"",
)

###############################################################################################
# Basic environment object. All the other environments will be based on this.
###############################################################################################

env = Environment(
	options = options,
	GANDER_MAJOR_VERSION = "0",
	GANDER_MINOR_VERSION = "0",
	GANDER_PATCH_VERSION = "0",
)

for e in env["ENV_VARS_TO_IMPORT"].split() :
	if e in os.environ :
		env["ENV"][e] = os.environ[e]

if env["PLATFORM"] == "darwin" :
	env["ENV"]["MACOSX_DEPLOYMENT_TARGET"] = "10.4"
	env.Append( CXXFLAGS = [ "-D__USE_ISOC99" ] )
	
if env["PLATFORM"] == "posix" :
	## We really want to not have the -Wno-strict-aliasing flag, but it's necessary to stop boost
	# python warnings that don't seem to be prevented by including boost via -isystem even. Better to
	# be able to have -Werror but be missing one warning than to have no -Werror.
	## \todo This is probably only necessary for specific gcc versions where -isystem doesn't
	# fully work. Reenable when we encounter versions that work correctly.
	env.Append( CXXFLAGS = [ "-Wno-strict-aliasing" ] )

if env["BUILD_CACHEDIR"] != "" :
	CacheDir( env["BUILD_CACHEDIR"] )

###############################################################################################
# Verify that we're either trying to build and package the dependencies with Gander, /or/
# trying to build against libraries installed elsewhere, but not both.
###############################################################################################

buildingDependencies = False
locatingDependencies = False
for o in options.options :
	if o.key.startswith( "BUILD_DEPENDENCY" ) and str( env.subst( "$" + o.key ) ) != str( env.subst( o.default ) ) :
		buildingDependencies = True
	elif o.key.startswith( "LOCATE_DEPENDENCY" ) and str( env.subst( "$" + o.key ) ) != str( env.subst( o.default ) ) :
		locatingDependencies = True

if buildingDependencies and locatingDependencies :
	raise RuntimeError( "Cannot specify BUILD_DEPENDENCY_* variables and LOCATE_DEPENDENCY* variables." )

###############################################################################################
# Dependencies
# They doesn't fit into the SCons way of things too well so we just build them directly when
# the script runs.
###############################################################################################

depEnv = env.Clone()

depEnv["ENV"].update(
	{
		"PATH" : depEnv.subst( "$BUILD_DIR/bin:" + os.environ["PATH"] ),
		"PYTHONPATH" : depEnv.subst( "$BUILD_DIR/python" ),
		"PKG_CONFIG_PATH" : depEnv.subst( "$BUILD_DIR/lib/pkgconfig" ),
		"CMAKE_PREFIX_PATH" : depEnv.subst( "$BUILD_DIR" ),
		"HOME" : os.environ["HOME"],
	}
)

if depEnv["PLATFORM"]=="darwin" :
	depEnv["ENV"]["DYLD_LIBRARY_PATH"] = depEnv.subst( "/System/Library/Frameworks/ApplicationServices.framework/Versions/A/Frameworks/ImageIO.framework/Resources:$BUILD_DIR/lib" )
	depEnv["ENV"]["DYLD_FALLBACK_FRAMEWORK_PATH"] = depEnv.subst( "$BUILD_DIR/lib" )
else :
	depEnv["ENV"]["LD_LIBRARY_PATH"] = depEnv.subst( "$BUILD_DIR/lib" )

def runCommand( command ) :

	command = depEnv.subst( command )
	sys.stderr.write( command + "\n" )
	subprocess.check_call( command, shell=True, env=depEnv["ENV"] )

if depEnv["BUILD_DEPENDENCY_BOOST"] :
	runCommand( "cd $BOOST_SRC_DIR; ./bootstrap.sh --prefix=$BUILD_DIR && ./bjam -d+2 variant=release link=shared threading=multi install")

if depEnv["BUILD_DEPENDENCY_EIGEN"] :
	runCommand( "mkdir -p $BUILD_DIR/Eigen && cp -R $EIGEN_SRC_DIR/Eigen $BUILD_DIR/Eigen/ && cp -R $EIGEN_SRC_DIR/unsupported $BUILD_DIR/Eigen/" )

###############################################################################################
# The basic environment for building libraries
###############################################################################################

if buildingDependencies :
	boostLibSuffix = ""
else :
	boostLibSuffix = env["BOOST_LIB_SUFFIX"]

baseLibEnv = env.Clone()

baseLibEnv.Append(

	CPPPATH = [
		"include",
	] + env["LOCATE_DEPENDENCY_CPPPATH"],
	
	CPPFLAGS = [
		"-DBOOST_FILESYSTEM_VERSION=3",
	],
	
	LIBPATH = [
		"./lib",
		"$BUILD_DIR/lib",
		"$LOCATE_DEPENDENCY_LIBPATH",
	],
	
	LIBS = [
		"pthread",
		"boost_iostreams" + boostLibSuffix,
		"boost_filesystem" + boostLibSuffix,
		"boost_date_time" + boostLibSuffix,
		"boost_thread" + boostLibSuffix,
		"boost_wave" + boostLibSuffix,
		"boost_regex" + boostLibSuffix,
		"boost_system" + boostLibSuffix,
	],
	
)

# include 3rd party headers with -isystem rather than -I.
# this should turns off warnings from those headers, allowing us to
# build with -Werror. there are so many warnings from boost
# in particular that this would be otherwise impossible - note that
# we're still having to turn off strict aliasing warnings in the
# default CXXFLAGS because somehow they creep out of boost python
# and past the defences.
for path in [
		"$BUILD_DIR/include",
		"$BUILD_DIR/include/Eigen",
	] + env["LOCATE_DEPENDENCY_SYSTEMPATH"] :
	
	baseLibEnv.Append(
		CXXFLAGS = [ "-isystem", path ]
	)

###############################################################################################
# Definitions for the libraries we wish to build
###############################################################################################

libraries = {
	"Gander" : {
		"envAppends" : {
			"LIBS" : [
			],
		}
	},
	"GanderTestModule" : {
		"install" : False,
		"envAppends" : {
			"CPPFLAGS" : [
				"-DBOOST_TEST_DYN_LINK=1",
			],
			"LIBS" : [
				"boost_test_exec_monitor" + boostLibSuffix,
			],
		}
	}
}

###############################################################################################
# Definitions for the tests we wish to build
###############################################################################################

tests = {
	"GanderTest" : {
		"envAppends" : {
			"CPPFLAGS" : [
				"-DBOOST_TEST_DYN_LINK=1",
			],
			"LIBS" : [
				"Gander",
				"GanderTestModule",
				"boost_test_exec_monitor" + boostLibSuffix
			],
		},
	},
}

###############################################################################################
# Tool to build the libraries
###############################################################################################

for libraryName, libraryDef in libraries.items() :

	# skip this library if we don't have the config we need
	haveRequiredOptions = True
	for requiredOption in libraryDef.get( "requiredOptions", [] ) :
		if not env[requiredOption] :
			haveRequiredOptions = False
			break
	if not haveRequiredOptions :
		continue

	# environment
	libEnv = baseLibEnv.Clone()
	libEnv.Append( **(libraryDef.get( "envAppends", {} )) )

	# library
	librarySource = sorted( glob.glob( "src/" + libraryName + "/*.cpp" ) )
	if librarySource :
	
		library = libEnv.SharedLibrary( "lib/" + libraryName, librarySource )
		libEnv.Default( library )

		if libraryDef.get( "install", True ) :	
			libraryInstall = libEnv.Install( "$BUILD_DIR/lib", library )
			libEnv.Alias( "build", libraryInstall )
	
	# header install
	headerInstall = libEnv.Install(
		"$BUILD_DIR/" + "include/" + libraryName,
		glob.glob( "include/" + libraryName + "/*.h" ) +
		glob.glob( "include/" + libraryName + "/*.inl" )
	)
	libEnv.Alias( "build", headerInstall )

###############################################################################################
# Testing
###############################################################################################

def preprocessor( target, source, env ) :
	import shutil

	try :
		tokens = env["SEARCH_REPLACE"]
	except :
		return 0

	targetPath = target[0].abspath
	sourcePath = source[0].abspath
	testName = os.path.splitext( os.path.basename( sourcePath ) )[0]
	targetDir = os.path.dirname( targetPath )

	if not os.path.exists( targetDir ) :
		op.path.mkdirs( targetDir )
	
	shutil.copyfile( sourcePath, targetPath )
	
	for search, replace in tokens.iteritems() :
		sedArgs = [ "sed", "-i", "s\\"+search+"\\"+replace+"\\g", targetPath ]
		if os.spawnvpe( os.P_WAIT, 'sed', sedArgs, os.environ ) != 0 :
			return 1

# The stuff that actually builds the tests

def unittest( target, source, env ) :
	testPath = str( source[0].abspath )
	testModule = os.path.basename( testPath )
	testDef = tests[testModule]
	
	env = baseLibEnv.Clone()
	env.PrependENVPath( 'LD_LIBRARY_PATH', ":".join( [ (env.Dir(i).abspath).replace(" ", ":" ) for i in env["LIBPATH"] ] ) )

	testCase = env["TestCase"]
	if testCase != "" :	
		testCase = "--run_test="+testCase
	
	if os.spawnve( os.P_WAIT, testPath, [ testModule, testCase ], env["ENV"] ) == 0 :
		open(str(target[0]),'w').write("PASSED\n")
	else:
		return 0

# Create a builder for tests
baseTestEnv = baseLibEnv.Clone()
bld = Builder( action = unittest )
baseTestEnv.Append( BUILDERS = {'Test' :  bld} )
bld = Builder( action = preprocessor )
baseTestEnv.Append( BUILDERS = {'TestPreprocessor' :  bld} )

for testModule, testDef in tests.items() :

	# skip this test if we don't have the config we need
	haveRequiredOptions = True
	for requiredOption in testDef.get( "requiredOptions", [] ) :
		if not env[requiredOption] :
			haveRequiredOptions = False
			break
	if not haveRequiredOptions :
		continue

	# environment
	libEnv = baseTestEnv.Clone()
	libEnv.Append( **(testDef.get( "envAppends", {} )) )

	objs = []
	
	# Compile each source file in the module by copying the source, doing a basic search/replace on various tokens and
	# build an Object file. Finally we build an executable from the objs.
	moduleSource = sorted( glob.glob( "src/" + testModule + "/*.cpp" ) )
	if moduleSource :
		for source in moduleSource :
			testName = os.path.splitext( os.path.basename( source ) )[0]
			testEnv = libEnv.Clone()
			testSource = source.replace( "src/", "test/src/" )
			testEnv["SEARCH_REPLACE"] = {}
			testEnv["SEARCH_REPLACE"]["LG_CORE_TEST_SUITE"] = testName
			testEnv["SEARCH_REPLACE"]["LG_CORE_TEST_MODULE"] = testModule

			testEnv.TestPreprocessor( testSource, source, preprocessor )
			objs.append( testEnv.Object( "test/" + testModule + "/" + testName, testSource ) )
	
	test = libEnv.Program( "test/" + testModule + "/" + testModule, objs )
	libEnv.Test( testModule + ".passed", test, libEnv )
	
	# add alias to run all unit tests.
	libEnv.Alias('test', testModule + ".passed")
	
	# To clean the tests we just remove the test directory.
	Clean( test, "test/src" )

#########################################################################################################
# Licenses
#########################################################################################################

if buildingDependencies :

	for l in [
		( "boost", "$BOOST_SRC_DIR/LICENSE_1_0.txt" ),
	] :
		license = env.InstallAs( "$BUILD_DIR/doc/licenses/" + l[0], l[1] )
		env.Alias( "build", license )

#########################################################################################################
# Installation
#########################################################################################################

manifest = [
	"bin/Gander",
	"LICENSE",
	"lib/libboost_thread" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_wave" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_regex" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_python" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_date_time" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_filesystem" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_iostreams" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libboost_system" + boostLibSuffix + "$SHLIBSUFFIX*",
	"lib/libGander*$SHLIBSUFFIX",
	"lib/libLG*",
	"include/Gander*",
	"include/boost",
]

def installer( target, source, env ) :

	def copyTree( src, dst, regex ) :
	
		names = os.listdir( src )
		
		for name in names:
				
			srcName = os.path.join( src, name )
			dstName = os.path.join( dst, name )
						
			if os.path.isdir( srcName ) :
				if regex.match( srcName ) :
					copyTree( srcName, dstName, re.compile( ".*" ) )
				else :
					copyTree( srcName, dstName, regex )
			else :
				if regex.match( srcName ) :
					dstDir = os.path.dirname( dstName )
					if not os.path.isdir( dstDir ) :
						os.makedirs( dstDir )
					if os.path.islink( srcName ) :
						os.symlink( os.readlink( srcName ), dstName )
					else:
						shutil.copy2( srcName, dstName )
				
	regex = re.compile( "|".join( [ fnmatch.translate( env.subst( "$BUILD_DIR/" + m ) ) for m in manifest ] ) )	
	copyTree( env.subst( "$BUILD_DIR" ), env.subst( "$INSTALL_DIR" ), regex )

install = env.Command( "$INSTALL_DIR", "$BUILD_DIR", installer )
env.AlwaysBuild( install )
env.NoCache( install )

env.Alias( "install", install )

#########################################################################################################
# Packaging
#########################################################################################################

def packager( target, source, env ) :

	installDir = env.subst( "$INSTALL_DIR" )
	b = os.path.basename( installDir )
	d = os.path.dirname( installDir )
	runCommand( env.subst( "tar -czf $PACKAGE_FILE -C %s %s" % ( d, b ) ) )
	
package = env.Command( "$PACKAGE_FILE", install, packager )
env.NoCache( package )
env.Alias( "package", package )
