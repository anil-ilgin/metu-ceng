# CENG488 - Guided Research


## ProBinDeePred : a Web-Service for Protein - Drug Binding Affinity-Prediction Based on Deep Learning Study

In this study, a web service to get bioactivity values of target protein – compound pairs either proven in a bioassay
or predicted through a learning model. In the proposed system,
[MDeePred](https://github.com/cansyl/MDeePred) study is used for predicting target protein – compound
pairs with improved performance and refined hyperparameters
to produce specific prediction models for each target protein
family. 

[MDeePred](https://github.com/cansyl/MDeePred) is a novel multi-channel protein-drug binding
affinity prediction method using several features of both proteins
and compounds. ECFP4 fingerprints for compound and sequential,
structural, evolutionary, and physicochemical properties are used
for proteins. To not repeat or predicting existing bioactivities, a
comprehensive database named [CROssBAR](https://cansyl.metu.edu.tr/crossbar) is used as a source
of information to provide users existing bioactivities. The trained
models are used to predict a bioactivity value for the protein-drug
pairs that don’t exist in the database. In the end, this comprehensive
web service provides end-users easy-to-use, reliable information
about given protein-drug pairs bioactivities.

### Tech Stack
#### Backend
- [Flask 1.1.2](https://github.com/pallets/flask)
- [SQLite](https://www.sqlite.org/index.html)
- [Chembl Websource Client](https://github.com/chembl/chembl_webresource_client)
- [Rdkit](https://www.rdkit.org/)
### Frontend Styling
- [Bootstrap](https://getbootstrap.com/docs/5.0/getting-started/introduction/)


Overview:

![Overview of ProBinDeePred](https://github.com/anil-ilgin/metu-ceng/blob/main/CENG-488/mdeepred-web/gui.png "Web-Service Overview")



This is an undergraduate research done in 2020-2021 Spring Semester @METU being part of CENG488 Guided Research course.

Term paper : [ProBinDeePred : a web-service for protein-drug binding affinity prediction](https://github.com/anil-ilgin/metu-ceng/blob/14e54fb9a5819b91d616a60ab3e81a5f6ce28b06/CENG-488/Term%20Paper%20-%20ProBinDeePred.pdf)

Symposium Link : [GUIDED2021: 3rd Guided Research Symposium ](https://saksagan.ceng.metu.edu.tr/courses/ceng488/index.html)
