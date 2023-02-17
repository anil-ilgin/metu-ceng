Kullanma Kılavuzu
hw2.py param1 param2 param3 param4 param5 param6
hw2.py dense_sift train_per_cat/validation_per_cat mode kmeans dense_window kNN

Parameter Explanations:
	dense_sift : DENSE or SIFT mode
	train_per_cat/validation_per_cat : images per category to be used
	mode : training or validation mode
	kmeans : k-means value
	dense_window : grid pixel width for Dense-SIFT
	kNN : Nearest neighbor count for kNN algorithm.

for SIFT training
	hw2.py SIFT 1-500 train 1-1024 _ _

for DENSE training
	hw2.py DENSE 1-500 train 1-1024 4-32 _

for SIFT test
	hw2.py SIFT 1-100 validation 1-1024 _ 1-N

for DENSE test
	hw2.py DENSE 1-100 validation 1-1024 4-32 1-N

Examples 

training SIFT with 
	-100 image per category,
	-kmeans value of 256 	
: hw2.py SIFT 100 train 256 _ _

training DENSE with
	-100 image per category,
	-kmeans value of 256,
	-grid size of 4 pixel width,
: hw2.py DENSE 100 train 256 4 _

validate SIFT with
	-100 image per category in validation set,
	-kmeans value of 256,
	-8 Nearest Neighbors
: hw2.py SIFT 100 validation 256 _ 8

validate DENSE with
	-100 image per category in validation set,
	-kmeans value of 256,
	-grid size of 4 pixel width,
	-8 Nearest Neighbors
: hw2.py DENSE 100 validation 256 4 8

default paths: 
	/the2.py
	/image_names.txt
	/clusters.txt
	/bof_database.txt
	/the2_data/validation/
	/the2_data/train/
	/test/

WARNING : must delete bof_database.txt and clusters.txt before running another training.


