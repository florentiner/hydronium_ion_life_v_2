# hydronium_ion_life_v_2
This is a training project that I did with Nikolay Sukhanov for a chemoinformatic course.

Goal of this project was to find the average lifetime of hydronium in silico experiments and compare it with the value of a real experiment. 

To reach this goal we used neural network n2p2 to simulate an environment with a considerable number of water molecules (360 molecules) and an added hydrogen cation.

After that, we had to write an algorithm analyzing the resulting file and determining when the hydrogen cation transfers to another water molecule. Based on the results of the algorithm, the necessary statistics are calculated. 

I wrote an algorithm on C++ trying to optimize the running time of the program, but due to time constraints, the algorithm was far from perfect. This repository contains an improved version of the algorithm.

More information about project and it results can be found in the Google presentation (https://docs.google.com/presentation/d/1cojQTGG-8JYVHib04xLLoeiYF0HFTsvFRbRrmog8Xjg/edit?usp=sharing)
