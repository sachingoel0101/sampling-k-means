data_name="mnist_no_label"
N=70000
k=10
directory="logs/"${data_name}"_"${k}"/"
mkdir -p $directory
for i in {1..20}
do
	echo ${i}_1
	./random_lloyd         "data/"${data_name} ${k} ${N}      | tee ${directory}${data_name}"_"${k}"_random_"${i}
	echo ${i}_2
	./kmp_lloyd            "data/"${data_name} ${k} ${N}      | tee ${directory}${data_name}"_"${k}"_kmp_"${i}
	echo ${i}_3
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 500 ${N}  | tee ${directory}${data_name}"_"${k}"_mean_500_"${i}
	echo ${i}_4
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 1000 ${N} | tee ${directory}${data_name}"_"${k}"_mean_1000_"${i}
	echo ${i}_5
	./mean_level2_d2_lloyd "data/"${data_name} ${k} 1500 ${N} | tee ${directory}${data_name}"_"${k}"_mean_1500_"${i}
done
rm -rf ${data_name}"_"${k}".txt"
echo "Initial cost" >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Initial cost" ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "Final cost" >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Final cost"   ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
echo "Iteration" >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_random"*    | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_kmp"*       | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_500"*  | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_1000"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
grep "Iteration"    ${directory}${data_name}"_"${k}"_mean_1500"* | cut -d':' -f3 | xargs >>${data_name}"_"${k}".txt"
