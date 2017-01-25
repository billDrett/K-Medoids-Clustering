<h1/>K-medoids Clustering Algorithm </h1>
<h2/>Introduction</h2> 
K-medoids is a clustering algorithm related to K-means. In contrast to the K-means algorithm, K-medoids chooses datapoints as centers of the clusters. There are eight combinations of Initialization, Assignment and Update algorithms to achieve the best results in the given dataset. Also Clara algorithm approach is implemented.

<h4/>Initialization</h4>
<lu/>
<li/>K-medoids++</li>
<li/>Concentrate(Park-Jun, paper)</li>
</lu>


<h4/>Assignment</h4>
<lu/>
<li/>PAM assignment(simplest approach) </li>
<li/>Assignment by LSH/DBH: Reverse Approach</li>
</lu>


<h4/>Update</h4>
<lu/>
<li/>Update a la Lloyd’s</li>
<li/>CLARANS update</li>
</lu>
<h4/>Clara Algorithm</h4>

Silhouette value is used to see the quality of the clusters.

<h2/>Support</h2>
<lu/>
<h4/>The K-medoids algorithm supports:</h4>
<li/>Euclidean  Vector Space</li>
<li/>Cosine Vector Space</li>
<li/>Hamming Metric Space</li>
<li/>Distance Matrix Metric Space</li>
</lu>
See the <a href="https://github.com/billDrett/K-Medoids-Clustering/blob/master/dataTypes.h">datatypes file</a> for the format of each metric space.

<h2/>Compile</h2>
Use make command to compile and make clean to delete object files(there is a MakeFile). 

<h2/>Run</h2>
./medoids –d &lt;input file> –c &lt;configuration file> -ο &lt;output file> -complete</br>
-d &lt;input file>: The input file name </br>

–c &lt;configuration file>: configuration file name</br>
-ο &lt;output file>: Output file name</br>
-complete: Output has the members of each cluster</br>
