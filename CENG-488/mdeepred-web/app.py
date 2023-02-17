from flask import Flask, render_template, request, redirect, url_for
from flask_sqlalchemy import SQLAlchemy
import os
import email_sender
import utility_functions as u_func

cwd = os.getcwd()
app = Flask(__name__, static_url_path='')
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///output_pairs.db'
db = SQLAlchemy(app)


class ProteinDrugPairPredictions(db.Model):
    drug_id = db.Column(db.String(100), primary_key=True, nullable=False, default="")
    protein_id = db.Column(db.String(100), primary_key=True, nullable=False, default="")
    affinity = db.Column(db.Float, nullable=False, default=0)

    def __repr__(self):
        return 'Drug ' + self.drug_id + ' Protein ' + self.protein_id


@app.route('/')
def home_page():
    return redirect("/affinity")


@app.route('/affinity', methods=['GET', 'POST'])
def affinity():
    if request.method == 'POST':
        # getting data from user forms in form of strings
        drugs, proteins, user_mail = request.form["drugs"], request.form["proteins"], request.form["email"]
        pro_family, drug_type, pro_type = request.form["protselect"], request.form["drugselect"], request.form[
            "prot_rep_select"]
        # splitting string of drugs and proteins into arrays
        new_drugs, new_proteins = drugs.split(","), proteins.split(",")
        # stripping leading/ending white space from inputs in case of any
        for i in range(len(new_drugs)):
            new_drugs[i] = new_drugs[i].strip()
        for i in range(len(new_proteins)):
            new_proteins[i] = new_proteins[i].strip()

        if drug_type == "chembl":
            # check is True if an error occured, else False
            check, smiles_list = u_func.drug_check(new_drugs)
        else:
            check, smiles_list= False, new_drugs

        # Checking whether input is erroneous or not
        if check:
            # return to main page showing erroneous info.
            return render_template('affinity.html', check=True)
        else:
            # since models are trained with uniprot, but proteins in database (CroSSBAR) is in form of chembl, getting both version of inputs.
            if pro_type == "uniprot":
                alternative = u_func.uniprot_to_chembl(new_proteins)
            else:
                alternative = u_func.chembl_to_uniprot(new_proteins)
                alternative, new_proteins = new_proteins, alternative
            # check2 is True if proteins are erroneous, returns main page with erroneous values
            check2, protein_arr = u_func.protein_check(alternative)
            if alternative == []:
                check2 = True
                for elem in new_proteins:
                    protein_arr.append(elem[0])
            if check2:
                return render_template('affinity.html', check=True)
        # a dictionary of pairs (chembl : uniprot) or (uniprot : chembl)
        protein_dict = {}
        for i in range(len(alternative)):
            protein_dict[alternative[i]] = new_proteins[i]
        # Cartesian product of protein and drug sets to get every possible drug-protein interaction of given values
        pairs = [(x, y) for x in new_drugs for y in new_proteins]

        # Get existing affinities from CRoSSBAR
        affinities_list = u_func.affinities_get(new_drugs, alternative, protein_dict)

        sent_files_path = "{}mdeepred-web/sent_files".format(cwd.split("mdeepred-web")[0])
        user_file_path = "{}/{}.csv".format(sent_files_path, user_mail)
        # to wipe the user_file content if it exists already
        open(user_file_path, "w").close()
        # opening output file for writing
        user_file = open(user_file_path, "a")

        # Write existing bioactivities in output file
        for elem in affinities_list:
            pair = (elem[2], elem[1])
            if pair in pairs:
                # write (Bioassay ID,drug ID,protein ID, p_chembl value) to csv file.
                user_file.write(elem[0] + "," + elem[2] + "," + elem[1] + "," + str(elem[3]) + "\n")
                # remove from pairs list to deduce prediction list.
                pairs.remove(pair)

        # Write existing predicted bioactivities in output file
        new_pairs = []
        for elem in pairs:
            # check predictions if they already done before
            db_check = ProteinDrugPairPredictions.query.filter_by(drug_id=elem[0], protein_id=elem[1]).first()
            if db_check:
                # write (PREDICTION, drug ID, protein ID, p_chembl value)
                user_file.write(
                    "PREDICTION," + db_check.drug_id + "," + db_check.protein_id + "," + str(db_check.affinity) + "\n")
            else:
                new_pairs.append(elem)

        # Make predictions if there are remaining pairs
        affinity_ = []
        if new_pairs:
            # create ecfp4 features for prediction model of remaining pairs
            u_func.create_ecfp4_features_given_smiles_dict(smiles_list, pro_family)
            affinity_ = u_func.predict_affinity(new_pairs, pro_family, "ecfp4_fe.tsv", user_mail)

        # Write newly predicted values both in file and local database
        for elem in affinity_:
            # add newly calculated predictions to local database
            new_pair = ProteinDrugPairPredictions(drug_id=elem[0], protein_id=elem[1], affinity=elem[2])
            db.session.add(new_pair)
            db.session.commit()
            user_file.write("PREDICTION," + elem[0] + "," + elem[1] + "," + str(elem[2]) + "\n")
        user_file.close()

        # send mail to user.
        email_sender.send_mail(user_mail)
        return redirect(url_for('affinity', check=False))
    else:
        return render_template('affinity.html', check=False)


if __name__ == '__main__':
    app.run(debug=True)
