import mdeepred_test
import urllib.parse
import urllib.request
import requests
import os
from chembl_webresource_client.new_client import new_client
from rdkit import Chem
from rdkit.Chem import AllChem
import pandas as pd

cwd = os.getcwd()


# takes a list of drugs as input, checks their validity from chembl.
# returns smiles representations of the drugs and erroneous inputs if any exist with a boolean indicating if it's erroneous or not.
def drug_check(drugs):
    smiles_list = {}
    molecule = new_client.molecule
    total = len(drugs)
    ret_arr = []
    total_arr = []
    sub_lists = [drugs[x:x+10] for x in range(0, len(range(total)), 10)]
    i = 0
    for ls in sub_lists:
        print(i)
        i += 1
        total_arr = molecule.get(ls)
        if (len(total_arr) != len(ls)):
            print("Anomaly in drugs array detected")
            return True, {}
        else:
            for j in range(len(total_arr)):
                smiles_list[ls[j]] = total_arr[j]['molecule_structures']['canonical_smiles']
    return False, smiles_list
# takes protein encodings and checks their validity from crossbar database.
# returns the protein list if it's valid, the error list if it's invalid with a boolean indicating if it's erroneous or not.
def protein_check(proteins):
    error_list = []
    session = requests.Session()
    for i in range(len(proteins)):
        if proteins[i] == "":
            return True, proteins
        parameters = {"limit": len(proteins), "targetIds": [proteins[i]]}
        resp = session.get("https://www.ebi.ac.uk/Tools/crossbar/targets", params=parameters)
        if resp.status_code != 200:
            return True, proteins
    return False, proteins

# Takes drug and protein id's along with a chembl:uniprot mapping of proteins, and gets all existing bioactivities from crossbar database.
# Returns the list of existing bioactivities as a list of triplets (Bioassay ID, target_protein chembl ID, molecule chembl ID)
def affinities_get(drug_ids, protein_ids, protein_dict):
    print('affinity get')
    parameters = {"limit": len(drug_ids) * len(protein_ids), "moleculeChemblId": drug_ids,
                  "targetChemblId": protein_ids}
    resp = requests.get("https://www.ebi.ac.uk/Tools/crossbar/activities", params=parameters)

    if resp and resp.status_code == 200:
        json_data = resp.json()
    else:
        json_data = None
    if json_data and "activities" in json_data:
        affinity_arr = []
        activities_arr = json_data["activities"]
        print('affinity size ', len(json_data["activities"]))
        for i in range(len(json_data["activities"])):
            print(i, end=',')
            affinity_arr.append((activities_arr[i]['assay_chembl_id'],
                                 protein_dict[activities_arr[i]['target_chembl_id']],
                                 activities_arr[i]['molecule_chembl_id'], activities_arr[i]['pchembl_value']))
        return affinity_arr
    else:
        return []


# takes a { smiles : chembl_id } and create ecfp4 vectors of them under the training folder
def create_ecfp4_features_given_smiles_dict(smiles_dict, training_data_name):
    #print(smiles_dict)
    str_header = "compound id\t" + "\t".join([str(num) for num in range(1024)])
    training_files_path = "{}mdeepred-web/training_files".format(cwd.split("mdeepred-web")[0])
    training_dataset_path = "{}/{}".format(training_files_path, training_data_name)
    comp_feature_vector_path = "{}/compound_feature_vectors".format(training_dataset_path)
    open(comp_feature_vector_path + "/ecfp4_fe.tsv", "w").close()
    filev = open(comp_feature_vector_path + "/ecfp4_fe.tsv", "a")
    filev.write(str_header + "\n")
    i = 0
    print('create ecfp')
    for comp_id in smiles_dict:
        print(i, end=',')
        i = i+1
        try:
            m = Chem.MolFromSmiles(smiles_dict[comp_id])
            fp = AllChem.GetMorganFingerprintAsBitVect(m, 2, nBits=1024).ToBitString()
            #print(comp_id + "\t" + "\t".join([str(float(dim)) for dim in fp]) + "\n")
            filev.write(comp_id + "\t" + "\t".join([str(float(dim)) for dim in fp]) + "\n")
        except:
            pass
    filev.close()


# takes pairs, protein family and file name of ecfp4 vectors and email of user and
# make predictions by loading appopriate model and feeding protein id & ecfp4 vectors into model.
# returns a list of predicted bioactivities
def predict_affinity(test_pairs, family, ecfp4_pth, email):
    pth_ = ""
    if family == "kinome":
        pth_ = "pretrained_kinome_model_state_dict.pth"
    elif family == "gcpr":
        pth_ = "pretrained_gpcr_model_state_dict.pth"
    elif family == "nuclear_receptor":
        pth_ = "pretrained_nuclear_receptor_model_state_dict.pth"
    training_files_path = "{}mdeepred-web/training_files".format(cwd.split("mdeepred-web")[0])
    training_dataset_path = "{}/{}/dti_datasets/model_inputs/".format(training_files_path, family)
    # .../mdeepred-web/training_files/family/dti_datasets_model_inputs/email.csv
    input_file = email + ".csv"
    input_path = training_dataset_path + input_file
    open(input_path, "w").close()
    f_input = open(input_path, "a")
    for elem in test_pairs:
        f_input.write(elem[0] + "," + elem[1] + "," + "0.00 \n")
    f_input.close()
    arr = mdeepred_test.load_model(family, pth_, ecfp4_pth, input_file)
    return arr


# takes uniprot encodings of proteins and translates them into chembl format using uniprot web service

def uniprot_to_chembl(ls_uniprot):
    url = 'https://www.uniprot.org/uploadlists/'
    query_uni = ""
    last = ls_uniprot[-1]
    for elem in ls_uniprot:
        query_uni += elem
        if elem != last:
            query_uni += "\t"
    params = {
        'from': 'ACC+ID',
        'to': 'CHEMBL_ID',
        'format': 'tab',
        'query': query_uni
    }
    data = urllib.parse.urlencode(params)
    data = data.encode('utf-8')
    req = urllib.request.Request(url, data)
    with urllib.request.urlopen(req) as f:
        response = f.read()
    x = response.decode('utf-8')
    y = x.splitlines()
    ret_arr = []
    for i in range(1, len(y)):
        z = y[i].split('\t')
        ret_arr.append(z[1])
    return ret_arr


# takes chembl id of proteins and translates them into uniprot format using uniprot web service

def chembl_to_uniprot(ls_chembl):
    url = 'https://www.uniprot.org/uploadlists/'
    query_uni = ""
    last = ls_chembl[-1]
    for elem in ls_chembl:
        query_uni += elem
        if elem != last:
            query_uni += "\t"
    params = {
        'from': 'CHEMBL_ID',
        'to': 'ACC+ID',
        'format': 'tab',
        'query': query_uni
    }
    data = urllib.parse.urlencode(params)
    data = data.encode('utf-8')
    req = urllib.request.Request(url, data)
    with urllib.request.urlopen(req) as f:
        response = f.read()
    x = response.decode('utf-8')
    y = x.splitlines()
    ret_arr = []
    for i in range(1, len(y)):
        z = y[i].split('\t')
        ret_arr.append(z[1])
    return ret_arr

def smiles_dict():
    pf = pd.read_csv("compound_smiles.tsv", sep="\t", header=0, low_memory=False, skipinitialspace=True)
    dictionary = {}
    print(pf.head())
    for index, row in pf.iterrows():
        print(index)
        id = row['chembl']
        smiles = row['smiles']
        dictionary[id] = smiles
    create_ecfp4_features_given_smiles_dict(dictionary,"kinome")