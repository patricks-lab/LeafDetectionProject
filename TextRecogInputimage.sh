#!/bin/bash

convEliminate=0
fileName=$1 #Filename to process

./TextRecognitionDatabaseToText $fileName #Make a database
numOfDataPoints=0
avg=0
whtspace=" "
dash="/"
args=" "

#Get convexity of current leaf
currconvexity=`cat ${fileName}.txt.convex`

for folder in *.database
do
   cd $folder

   #------CONVEXITY TEST BEGIN------
   mu=0 #Average
   sigma=0 #Standard deviation
   N=0 #size
   #Run convexity check -> see if leaf is OUTLIER!

   #First, calucalte average.
   for conv in *.convex
   do
     convexity=`cat ${conv}`
     mu=$(echo $mu + $convexity | bc)
     N=$((N+1))
   done

   mu=$(echo $mu / $N | bc)

   #Calculate standard deviation.
   for conv in *.convex
   do
     convexity=`cat ${conv}`
     sigma=$(echo "$sigma + ($convexity - $mu)^2" | bc)
   done

   #If there is one leaf, why bother doing standard deviation? Divide-by-0 error!
   if(( N != 1 ))
   then
     sigma=$(echo "sqrt($sigma/$((N-1)))" | bc)

     #Now calculate lower, upper outlier bounds to see if our image falls into it...
     lowerBound=$(echo "$mu-3*$sigma" | bc)
     upperBound=$(echo "$mu+3*$sigma" | bc)

     #echo "lb: ${lowerBound} up: ${upperBound}"

     #Check this image...
     isGreaterThanLower=$(echo "$currconvexity > $lowerBound" | bc)
     isLessThanUpper=$(echo "$currconvexity < $upperBound" | bc)
     if((($isGreaterThanLower != 1 || $isLessThanUpper != 1) && ($convEliminate == 1) ))
     then
       cd ../
       avg=0
       numOfDataPoints=0
       continue
     fi
   fi
   #------CONVEXITY TEST END--------

   for dataFile in *.png.txt
   do
      output=$(.././AbsoluteDistance ../${fileName}.txt $dataFile) #Run a pearson correlation
      output2=$(.././AbsoluteDistance ../${fileName}.txt.reversed $dataFile) #Run a pearson correlation (for reversed)

      oisgreat=$(echo "$output > $output2" | bc)
      #echo $oisgreat

       #Take the minimum of the comparisons....
       if(( $oisgreat != 1))
       then
         output=$output2
       fi

      numOfDataPoints=$((numOfDataPoints+1))
      #echo $numOfDataPoints
      avg=$(echo $avg + $output | bc)
   done

   #Take sum/#ofpoints -> average score
   avg=$(echo "$avg / $numOfDataPoints" | bc -l)

   #printf "Average of leaf ${folder} is:%0.2f percent\n" ${avg}
   printf "%0.2f percent similarity, leaf ${folder}\n" ${avg} >> ../${fileName}.results

   cd ../

   avg=0
   numOfDataPoints=0
done


#Sort the results and print out results (high -> low)
sort -r -o ${fileName}.results ${fileName}.results
cat ${fileName}.results

while IFS= read -r line
do
    fldr=${line:30}
    cd $fldr
    for image in *.png
    do
      args=$args$fldr$dash$image$whtspace
      break
    done
    cd ../
done < "${fileName}.results"


#echo $args
java graphic $args


rm ${fileName}.results



#Ask user if correct leaf?
read -p "Was the leaf correctly identified? Y for yes, or N for no." -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Nn]$ ]]
then
  echo "Then what database should it be added to, or what new database should be created?"
  echo "Also, give me the correct name of the leaf"
  read newDatabase

  mkdir -p "${newDatabase}.database"
  cd "${newDatabase}.database"

  #copy some files...
  cp ../$fileName $fileName
  cp ../"${fileName}.txt" "${fileName}.txt"
  cp ../"${fileName}.txt.convex" "${fileName}.txt.convex"
  cp ../"${fileName}.txt.reversed" "${fileName}.txt.reversed"

  cd ../
else
  #Also save the files to the database.
  echo "Thanks for using this app!"
fi

#Cleanup datafiles
rm "${fileName}.txt"
rm "${fileName}.txt.reversed"
rm "${fileName}.txt.convex"
