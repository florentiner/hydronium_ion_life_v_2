# hydronium_ion_life_v_2

![Grotthuss mechanism](https://upload.wikimedia.org/wikipedia/commons/d/d3/Proton_Zundel.gif)

# Project Description:

This is a training project that I (Danil Ezhov) did with my groupmate Nikolai Sukhanov as an examination project for the course of chemoinformatics.

Goal of this project was to find the average lifetime of hydronium in silico experiments and compare it with the value of a real experiment. 

To reach this goal we used neural network n2p2 to simulate an environment with a considerable number of water molecules (360 molecules) and an added hydrogen cation.

After that, we had to write an algorithm analyzing the resulting file and determining when the hydrogen cation transfers to another water molecule. Based on the results of the algorithm, the necessary statistics are calculated. 

I wrote an algorithm on C++ trying to optimize the running time of the program, but due to time constraints, the algorithm was far from perfect. This repository contains an improved version of the algorithm.

# Steps

1. Use n2p2 to generate trajectory file. 

Use official n2p2 documentation (https://compphysvienna.github.io/n2p2/) or use already generated trajectory file (https://drive.google.com/drive/folders/1171H9Y3JmubxmxBJFLCEM18LqhYzYVEE?usp=sharing).

Files for n2p2 input are located in run_n2p2 folder

If type of trajectory file will be ```.dmp``` just change it to ```.LAMMPSDUMP```

2. Download and build program that calculate lifetime and data for visualization.

```
git clone https://github.com/florentiner/hydronium_ion_life_v_2.git
cd hydronium_ion_life_v_2/analyzer
mkdir build && cd build
cmake .. && make
```

Alternatively build ```docker``` image locally:

Make ```Dockerfile```:

```
cat << EOF > Dockerfile  
# syntax=docker/dockerfile:1
FROM alpine:3.15
WORKDIR /app
RUN apk add git build-base zlib-dev cmake && \
    git clone http://github.com/florentiner/hydronium_ion_life_v_2.git && \
    cd hydronium_ion_life_v_2/analyzer && \
    mkdir build && cd build && \
    cmake .. && make

FROM alpine:3.15
WORKDIR /app 
RUN apk add build-base zlib-dev --no-cache bash && \
    mkdir data
COPY --from=0 /app/hydronium_ion_life_v_2/analyzer/build ./
ENTRYPOINT ["./hydro_ion_life_v_2", "-v=/app/data/", "-t=all"]
EOF
```

Build image:

```
docker build -t hydrogen_life_calculator .
```


3. Run programe with trajectory.

```
./hydro_ion_life_v_2 <path/to/trajectory/file/trajectory_file_name> # run on 1 thred
./hydro_ion_life_v_2 -t=40 <path/to/trajectory/file/trajectory_file_name> # run on 40 threds
./hydro_ion_life_v_2 -t=all <path/to/trajectory/file/trajectory_file_name> # run on all threds
./hydro_ion_life_v_2 -v=<path/to/directory/with/results> <path/to/trajectory/file/trajectory_file_name> # specify directory where results will be saved
```

Alternatively use ```docker```:

 * Localy:

```
docker run --rm -v <path/to/trajectory/file>:/app/data hydrogen_life_calculator data/<trajectory_file_name> # run on all CPUs resources*
docker run --rm -v <path/to/trajectory/file>:/app/data --cpus="0.5" hydrogen_life_calculator data/<trajectory_file_name> # limit CPUs resources to half of core
```
*notice that ```docker``` operate CPUs resources, not threads. For example 0.5 CPUs resources on CPU with 2 cores and 4 threads same as 1 thread.

 * With ```quay.io``` image:

```
docker run --rm -v <path/to/trajectory/file>:/app/data quay.io/florentiner/hydrogen_life_calculator data/<trajectory_file_name>
docker run --rm -v <path/to/trajectory/file>:/app/data --cpus="0.5" quay.io/florentiner/hydrogen_life_calculator data/<trajectory_file_name> # limit CPUs resources to half of core
```

Programe will generate:

* ```res_life.txt``` file with information of hydronium lifetime.
* ```vis_mod.txt``` file with information to visualize model of trajectory.

Or use already generated files (https://drive.google.com/drive/folders/1ZTA9SaqLIs6yKc80o01Hip82wujmmnGN?usp=sharing)

4. Visualize trajectory

To visualize trajectory use ```visualization_model_of_water_dynamic.ipynb``` in visualization_of_water_modeling folder

**Example Visualistation of modeling:**

[![Visualistation of modeling](https://img.youtube.com/vi/L8atFMG-Ka0/0.jpg)](https://youtu.be/L8atFMG-Ka0)

5. Make statistics.

To make statistics use ```Hist.ipynb``` in make_statistic folder

# Links:

<h4> More information about project and it results can be found in the Google presentation: https://docs.google.com/presentation/d/1cojQTGG-8JYVHib04xLLoeiYF0HFTsvFRbRrmog8Xjg/edit?usp=sharing </h4>

<h4> Text for presentation: https://docs.google.com/document/d/15O3OGPX84zqZUGAQ4JAzSDecWgfmfsFjwp-7InwxR2Q/edit?usp=sharing </h4>

<h4> Link to older version of project: https://github.com/florentiner/hydronium_ion_life </h4>

<h4> My contacts: email: danil.ejow98@gmail.com telegram: @decoratoren </h4>
