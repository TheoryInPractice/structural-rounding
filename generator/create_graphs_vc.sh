#!/bin/bash

edges=$1
dir=$2

mkdir -p $dir

for balance in 1 2 10 100
do
	for octsize in .01 .05 .1 .25 .4
	do
		for bbdens in .01 .001 .005
		do
			for oodens in .05 .001 .01
			do
				for obdens in .01 .05
				do
					for cv in .5 1.5
					do
						o=`echo "$octsize*($balance + 1)/(1 - $octsize)" | bc -l`

						bbedges=`echo "$balance * $bbdens" | bc -l`
						obedges=`echo "$o * ($balance + 1) * $obdens" | bc -l`
						ooedges=`echo "$o * $o * $oodens / 2" | bc -l`

						left=`echo "sqrt($edges / ($bbedges + $obedges + $ooedges))" | bc`
						right=$(($left * $balance))
						oct=`echo "$o * $left" | bc`

						mean=`echo "$left * $bbdens" | bc -l`
						std=`echo "$mean / 2" | bc -l`

						for seed in 0 1 2 3 4
						do
							name=${dir}/m${edges}_bal${balance}_oo${oodens}_ob${obdens}_bb${bbdens}_oct${octsize}_cv${cv}_copy${seed}
							./generator/generator.out ${left} ${right} ${mean} ${std} ${oct} ${oodens} ${obdens} ${cv} ${seed} ${name}
							echo ${name}
						done
					done
				done
			done
		done
	done
done
