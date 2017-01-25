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

<h2>Functionality</h2>
The input date are clustered in k clusters. The k-medoids clustering works like this.
1)Initialize centroids </br>
2)Assign the rest of the points to the centroids</br>
3)Update the centroids</br>
Go to 2 until no better clusters are found</br>
All initialization, assign and update methods are used and the results of the clusters are writed to the output file as with the silhouette value of each cluster
<h2/>K-medoids API</h2>
void KMedoids&lt;T>::run(InitializationType initial, AssignmentType Assign, UpdateType update, int s = 2)</br>
initial is ={InitializationPP,  InitializationConcentrate} </br>
Assign is ={PamAssign,  LSHAssign}</br>
update is={Lloyds,Clarans}</br>
void KMedoids&lt;T>::clara(int s = 5) </br> 
is the clara algorithm, and s is the number of iterations



<h2>Files</h2>
Format of input file:</br>
<b>Vector space</b></br>
@metric_space vector </br>
@metric {euclidean, cosine} //default: euclidean</br>
item_id1  x11 x12 ... x1d</br>
item_id2</br>
.</br>
.</br>
item_idN  xN1 xN2 ... xNd</br></br>
<b>Hamming</b></br>
@metric_space hamming</br>
item_id1  B1</br>
....</br>
item_idN</br></br>
<b>Distance Matrix</b></br>
@metric_space matrix </br>
item_id1  x11 x12 ... x1d</br>
item_id2</br>
.</br>
.</br>
item_idQ  xQ1 xQ2 ... xQd</br></br>
<b>Configuration file</b></br>
number_of_clusters: int // k value</br>
number_of_hash_functions: int //default 4</br>
number_of_hash_tables: int //default 5</br>
clarans_set_fraction: int //default max{0.12*k(N-k),250}, k is the number of clusters and N the number of the datapoints </br>
clarans_iterations: int //default 2</br>


