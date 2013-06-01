#!/bin/bash

GIT=git
GREP=grep
AWK=awk
SED=sed
lasttag=`$GIT describe --tags --abbrev=0`
branch=`$GIT branch |$GREP '*'|$AWK '{print $2}'`
devpart=`date +"%Y%m%d%H%M"`

if [ "$branch" == "master" ]; then
	echo "$lasttag"
elif [ "$branch" == "develop" ]; then
	lasttag=`$GIT describe --tags --abbrev=0`
	nparts=$((`echo $lasttag | $SED 's/\./ /g'|tr ' ' '\n'|wc -l`))
	echo "Develop branch"

	echo -n "Version: $lasttag"
	newversion=""
	if [ $nparts -lt 4 ]; then
		newversion=$lasttag.$devpart
	else 
		lasttag=`$GIT describe --tags --abbrev=0`
		parts=(${lasttag//./ })
		MAJOR=$((${parts[0]}))
		MINOR=$((${parts[1]}))
		FIX=$((${parts[2]}))
		newversion=$MAJOR.$MINOR.$FIX.$devpart
	
		if [ "$newversion" == "$lasttag" ]; then
			echo "WARNING! Tag is same as last one!"
		fi
	fi

	echo -e "\nNew version: $newversion"
else
	# Feature, Hotfix or Release
	subbranch=`echo $branch|$SED 's/\// /'|$AWK '{print $1}'`
	otherpart=`echo $branch|$SED 's/\// /'|$AWK '{print $2}'`
	if [ "$subbranch" == "release" ]; then
		echo "Release branch, no tagging needed"
		echo "Version: $otherpart"
	elif [ "$subbranch" == "hotfix" ]; then
		echo "Hotfix branch (new version relative to master)"
		lasttag=`$GIT describe --tags --abbrev=0 master`

		parts=(${lasttag//./ })
		nparts=${#parts[@]}
		if [ $nparts -eq 3 ]; then
			MAJOR=$((${parts[0]}))
			MINOR=$((${parts[1]}))
			FIX=$((${parts[2]}))

			$NFIX=$(($FIX+1))
			echo "New version: $MAJOR.$MINOR.$NFIX"
		else
			echo "Master has incorrect version number!"
		fi
	else
		echo "No version number!"
	fi
fi
