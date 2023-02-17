sys.path.append('./MDeePred/bin')
import os
import warnings

import numpy as np
import torch
from torch.autograd import Variable
from data_processing import get_cnn_test_data_loader3
from models import get_model

warnings.filterwarnings(action='ignore')

cwd = os.getcwd()
project_file_path = "{}mdeepred-web".format(cwd.split("mdeepred-web")[0])


def load_model(training_data_name, model_fl, ecfp4_fl, input_file):
    torch.manual_seed(123)
    np.random.seed(123)
    use_gpu = torch.cuda.is_available()

    device = "cpu"

    if use_gpu:
        print("GPU is available on this device!")
        device = "cuda"
    else:
        print("CPU is available on this device!")

    if training_data_name == 'nuclear_receptor':
        comp_h_1 = 1024  # int(params[0].split("_")[0])  # 512#int(params[0])
        comp_h_2 = 1024  # int(params[0].split("_")[1])  # 512#int(params[1])
        tar_after_flat = 256  # int(params[1])  # 64#int(params[2])
        last_h_1 = 1024  # int(params[2].split("_")[0])  # 256#int(params[3])
        last_h_2 = 1024  # int(params[2].split("_")[1])  # 128#int(params[4])
        learn_rate = 0.0001  # float(params[3])  # 0.0001#float(params[5])
        batch_size = 128  # int(params[4])  # 32#int(params[6])
        model_name = "CompFCNNTarCNNModuleInception"  # params[7]
        dropout = 0.3  # float(params[10])  # 0.1 # float(params[8])
        tar_feats = "sequencematrix500_ZHAC000103LEQ500_GRAR740104LEQ500_SIMK990101LEQ500_blosum62LEQ500"
    else:
        comp_h_1 = 1024  # int(params[0].split("_")[0])  # 512#int(params[0])
        comp_h_2 = 1024  # int(params[0].split("_")[1])  # 512#int(params[1])
        tar_after_flat = 256  # int(params[1])  # 64#int(params[2])
        last_h_1 = 1024  # int(params[2].split("_")[0])  # 256#int(params[3])
        last_h_2 = 512  # int(params[2].split("_")[1])  # 128#int(params[4])
        learn_rate = 0.0001  # float(params[3])  # 0.0001#float(params[5])
        batch_size = 32  # int(params[4])  # 32#int(params[6])
        model_name = "CompFCNNTarCNNModuleInception"  # params[7]
        dropout = 0.1  # float(params[10])  # 0.1 # float(params[8])
        tar_feats = "sequencematrix1000_ZHAC000103LEQ1000_GRAR740104LEQ1000_SIMK990101LEQ1000_blosum62LEQ1000"

    model = get_model("CompFCNNTarCNNModuleInception", tar_feats.split("_"), 1024, tar_after_flat, comp_h_1, comp_h_2,
                      last_h_1, last_h_2, dropout)

    if use_gpu:
        model.load_state_dict(
            torch.load("{}/trained_models/{}/{}".format(project_file_path, training_data_name, model_fl)))
    else:
        model.load_state_dict(
            torch.load("{}/trained_models/{}/{}".format(project_file_path, training_data_name, model_fl),
                       map_location='cpu'))
    # model.load_state_dict(torch.load("{}/trained_models/kinome/{}".format(project_file_path, model_fl)))
    model = model.to(device)
    model.eval()

    test_loader = get_cnn_test_data_loader3(training_data_name, ["ecfp4"], tar_feats.split("_"), ecfp4_fl, input_file)
    # test_loader = get_cnn_test_data_loader2(training_data_name, ["ecfp4"], tar_feats.split("_"), ecfp4_fl)
    test_predictions, test_labels, test_all_comp_ids, test_all_tar_ids = [], [], [], []
    total_test_count = 0

    model.eval()
    with torch.no_grad():  # torch.set_grad_enabled(False):

        for i, data in enumerate(test_loader):
            test_comp_feature_vectors, test_target_feature_vectors, tst_labels, test_compound_ids, test_target_ids = data
            test_comp_feature_vectors, test_target_feature_vectors, tst_labels = Variable(test_comp_feature_vectors).to(
                device), Variable(
                test_target_feature_vectors).to(device), Variable(tst_labels).to(device)

            total_test_count += test_comp_feature_vectors.shape[0]

            test_y_pred = None
            test_y_pred = model(test_comp_feature_vectors, test_target_feature_vectors)
            for item in tst_labels:
                test_labels.append(float(item.item()))

            for item in test_y_pred:
                test_predictions.append(float(item.item()))

            for item in test_compound_ids:
                test_all_comp_ids.append(item)

            for item in test_target_ids:
                test_all_tar_ids.append(item)
        # print(test_all_tar_ids)
        print_predictions = True
        output_path = "{}/prediction_outputs/{}".format(project_file_path, input_file)
        open(output_path, "w").close()
        filev = open(output_path, "a")
        if print_predictions:
            ret_arr = []
            for ind in range(len(test_all_tar_ids)):
                filev.write(
                    "{},{},{}".format(test_all_comp_ids[ind], test_all_tar_ids[ind], test_predictions[ind]) + "\n")
                ret_arr.append((test_all_comp_ids[ind], test_all_tar_ids[ind], test_predictions[ind]))
            filev.close()
            return ret_arr

# python mdeepred_test.py kinome pretrained_kinome_model_state_dict.pth test_unseen_ecfp4_normalized.tsv

# test_unseen_ecfp4_normalized.tsv dataset should be under /dti_datasets path
# kinome is dataset's name
# pretrained_kinome_model_state_dict.pth should be under MDeePred/trained_models/kinome payh
