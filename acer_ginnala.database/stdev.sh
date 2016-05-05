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

sigma=$(echo "sqrt($sigma/$((N-1)))" | bc)

lowerBound=$(echo "$mu-2.5*$sigma" | bc)
upperBound=$(echo "$mu+2.5*$sigma" | bc)

echo "lb: ${lowerBound} up: ${upperBound}"
