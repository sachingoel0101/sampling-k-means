data_name="3d_pro"
N=434874
k=5
directory="logs/"${data_name}"_"${k}"/"
mkdir -p $directory
for i in {3..5}
do
	./random_lloyd         "data/"${data_name} ${k} ${N}      | tee ${directory}${data_name}"_"${k}"_random_"${i}
	./kmp_lloyd            "data/"${data_name} ${k} ${N}      | tee ${directory}${data_name}"_"${k}"_kmp_"${i}
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 500 ${N}  | tee ${directory}${data_name}"_"${k}"_mean_500_"${i}
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 1000 ${N} | tee ${directory}${data_name}"_"${k}"_mean_1000_"${i}
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 1500 ${N} | tee ${directory}${data_name}"_"${k}"_mean_1500_"${i}
done
rm ${data_name}"_"${k}".txt"
echo "Random" >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "K means ++">>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "Mean N 500">>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "Mean N 1000">>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "Mean N 1500">>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
