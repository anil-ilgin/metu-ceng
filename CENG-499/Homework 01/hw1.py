import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.transforms as T
from torchvision.datasets import CIFAR10
from torch.utils.data import random_split, DataLoader
from tqdm import tqdm

#ANIL UTKU ILGIN
#2260073

#Activation function should be softmax
#Loss function should be cross entropy
#Consider using a CrossEntropyLoss function for both.
# Do NOT put an activation function after the last fully-connected layer before the softmax operation.
#Can choose the Adam Optimizer
#Output layer should have 10 result
#Try 1,2,3 Layer at least. You can also use different number of layers
#Try different activation func at and of every layer.
#Try different learning rates.


#TWEAKING HYPERPARAMETERS

l_rate = [0.01, 0.001, 0.0001] #Learning Rates
layer_1 = [512, 768, 1024] #Hidden Layer Neuron Counts
layer_2= [(400,200), (512,256), (1024,512)] #Hidden Layers Neuron Counts (Layer1, Layer2)
b_size = 32 #Batch size
activation_f = ['T', 'S', 'R'] #Activation Functions, T: Tanh, S: Sigmoid, R: Relu
early_stop_max = 5 #After not improving 5 times since best validation score, stop.
epoch_count = 20 #Epoch count


#define the ANNs

class my_ANN_L1(nn.Module):
    def __init__(self):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=10)
    def forward(self, x):
        x = torch.flatten(x, 1)
        x = self.layer1(x)
        return x

class my_ANN_L2_relu(nn.Module):
    def __init__(self, num_layer1):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.relu(self.layer1(x))
        x = self.layer2(x)
        return x

class my_ANN_L2_tanh(nn.Module):
    def __init__(self, num_layer1):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.tanh(self.layer1(x))
        x = self.layer2(x)
        return x

class my_ANN_L2_sigmoid(nn.Module):
    def __init__(self, num_layer1):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.sigmoid(self.layer1(x))
        x = self.layer2(x)
        return x



class my_ANN_L3_relu(nn.Module):
    def __init__(self, num_layer1, num_layer2):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=num_layer2)
        self.layer3 = nn.Linear(in_features=num_layer2, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.relu(self.layer1(x))
        x = F.relu(self.layer2(x))
        x = self.layer3(x)
        return x


class my_ANN_L3_tanh(nn.Module):
    def __init__(self, num_layer1, num_layer2):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=num_layer2)
        self.layer3 = nn.Linear(in_features=num_layer2, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.tanh(self.layer1(x))
        x = F.tanh(self.layer2(x))
        x = self.layer3(x)
        return x

class my_ANN_L3_sigmoid(nn.Module):
    def __init__(self, num_layer1, num_layer2):
        super().__init__()
        self.layer1 = nn.Linear(in_features=32*32, out_features=num_layer1)
        self.layer2 = nn.Linear(in_features=num_layer1, out_features=num_layer2)
        self.layer3 = nn.Linear(in_features=num_layer2, out_features=10)

    def forward(self, x):
        x = torch.flatten(x, 1)
        x = F.sigmoid(self.layer1(x))
        x = F.sigmoid(self.layer2(x))
        x = self.layer3(x)
        return x

#Homework Part Deneme

device = 'cuda' if torch.cuda.is_available() else 'cpu'

#1 Layer ANN tester
def main_1layer(num_epochs, train_loader, val_loader, test_loader, lr_rate, files):
    
    model = my_ANN_L1().to(device)
    loss_function = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=lr_rate)
    early_stop_count = 0
    best_loss = None
    for epoch in tqdm(range(num_epochs)):
        # Training
        model.train()
        accum_train_loss = 0
        for i, (imgs, labels) in enumerate(train_loader, start=1):
            imgs, labels = imgs.to(device), labels.to(device)
            output = model(imgs)
            loss = loss_function(output, labels)

            # accumlate the loss
            accum_train_loss += loss.item()

            # backpropagation
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        
        # Validation
        model.eval()
        accum_val_loss = 0
        with torch.no_grad():
            for j, (imgs, labels) in enumerate(val_loader, start=1):
                imgs, labels = imgs.to(device), labels.to(device)
                output = model(imgs)
                accum_val_loss += loss_function(output, labels).item()
        
        new_val_loss = accum_val_loss / j
        if best_loss == None:
            best_loss = new_val_loss
        elif best_loss > new_val_loss:
            best_loss = new_val_loss
            early_stop_count = 0
        else:
            early_stop_count += 1
        
        if early_stop_count >= early_stop_max:
            files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            files.write("Training Early Stopped Here, Started Overfitting.")
            print("Training Early Stopped Here, Started Overfitting.")
            break

        # print statistics of the epoch
        files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
        print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')

        # Compute Test Accuracy
    model.eval()
    with torch.no_grad():
        correct = total = 0
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
            
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)
    files.write(f'Test Accuracy = {100 * correct/total :.3f}%')
    print(f'Test Accuracy = {100 * correct/total :.3f}%')

    with torch.no_grad():
        correct = total = 0
        for images, labels in val_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
                        
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)

    files.write(f'Val Accuracy = {100 * correct/total :.3f}%')
    print(f'Val Accuracy = {100 * correct/total :.3f}%')   


#2 Layer ANN tester
def main_2layer(val, num_epochs, train_loader, val_loader, test_loader, lr_rate, num_layer1, files):
    if val == 'S':
        model = my_ANN_L2_sigmoid(num_layer1).to(device)
    elif val == 'T':
        model = my_ANN_L2_tanh(num_layer1).to(device)
    elif val == 'R':
        model = my_ANN_L2_relu(num_layer1).to(device)
    else:
        return -1

    loss_function = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=lr_rate)
    early_stop_count = 0
    best_loss = None
    for epoch in tqdm(range(num_epochs)):
        # Training
        model.train()
        accum_train_loss = 0
        for i, (imgs, labels) in enumerate(train_loader, start=1):
            imgs, labels = imgs.to(device), labels.to(device)
            output = model(imgs)
            loss = loss_function(output, labels)

            # accumlate the loss
            accum_train_loss += loss.item()

            # backpropagation
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        
        # Validation
        model.eval()
        accum_val_loss = 0
        with torch.no_grad():
            for j, (imgs, labels) in enumerate(val_loader, start=1):
                imgs, labels = imgs.to(device), labels.to(device)
                output = model(imgs)
                accum_val_loss += loss_function(output, labels).item()
                
        new_val_loss = accum_val_loss / j
        if best_loss == None:
            best_loss = new_val_loss
        elif best_loss > new_val_loss:
            best_loss = new_val_loss
            early_stop_count = 0
        else:
            early_stop_count += 1
        
        if early_stop_count >= early_stop_max:
            files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            files.write("Training Early Stopped Here, Started Overfitting.")
            print("Training Early Stopped Here, Started Overfitting.")
            break
        

        # print statistics of the epoch
        files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
        print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')

        # Compute Test Accuracy
    model.eval()
    with torch.no_grad():
        correct = total = 0
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
            
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)

    files.write(f'Test Accuracy = {100 * correct/total :.3f}%')
    print(f'Test Accuracy = {100 * correct/total :.3f}%')

    with torch.no_grad():
        correct = total = 0
        for images, labels in val_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
                        
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)

    files.write(f'Val Accuracy = {100 * correct/total :.3f}%')
    print(f'Val Accuracy = {100 * correct/total :.3f}%')   
        
    
#3 Layer ANN tester
def main_3layer(val, num_epochs, train_loader, val_loader, test_loader, lr_rate, num_layer1, num_layer2, files):
    if val == 'S':
        model = my_ANN_L3_sigmoid(num_layer1, num_layer2).to(device)
    elif val == 'T':
        model = my_ANN_L3_tanh(num_layer1, num_layer2).to(device)
    elif val == 'R':
        model = my_ANN_L3_relu(num_layer1, num_layer2).to(device)
    else:
        return -1

    loss_function = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(model.parameters(), lr=lr_rate)
    early_stop_count = 0
    prev_loss = None
    for epoch in tqdm(range(num_epochs)):
        # Training
        model.train()
        accum_train_loss = 0
        for i, (imgs, labels) in enumerate(train_loader, start=1):
            imgs, labels = imgs.to(device), labels.to(device)
            output = model(imgs)
            loss = loss_function(output, labels)

            # accumlate the loss
            accum_train_loss += loss.item()

            # backpropagation
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        
        # Validation
        model.eval()
        accum_val_loss = 0
        with torch.no_grad():
            for j, (imgs, labels) in enumerate(val_loader, start=1):
                imgs, labels = imgs.to(device), labels.to(device)
                output = model(imgs)
                accum_val_loss += loss_function(output, labels).item()

                
        new_val_loss = accum_val_loss / j
        if prev_loss == None:
            prev_loss = new_val_loss
        elif new_val_loss < prev_loss:          
            prev_loss = new_val_loss
            early_stop_count = 0
        else:
            early_stop_count += 1
                
        if early_stop_count >= early_stop_max:
            files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
            files.write("Training Early Stopped Here, Started Overfitting.")
            print("Training Early Stopped Here, Started Overfitting.")
            break
 

        # print statistics of the epoch
        files.write(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')
        print(f'Epoch = {epoch} | Train Loss = {accum_train_loss / i:.4f}\tVal Loss = {accum_val_loss / j:.4f}')

        # Compute Test Accuracy
    model.eval()
    with torch.no_grad():
        correct = total = 0
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
            
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)
    files.write(f'Test Accuracy = {100 * correct/total :.3f}%')
    print(f'Test Accuracy = {100 * correct/total :.3f}%')

    with torch.no_grad():
        correct = total = 0
        for images, labels in val_loader:
            images, labels = images.to(device), labels.to(device)
            output = model(images)
                        
            _, predicted_labels = torch.max(output, 1)
            correct += (predicted_labels == labels).sum()
            total += labels.size(0)

    files.write(f'Val Accuracy = {100 * correct/total :.3f}%')
    print(f'Val Accuracy = {100 * correct/total :.3f}%')   



#Main loop for every parameter option for grid search
def main_loop():
    train_transform = T.Compose([
        T.ToTensor(), T.Grayscale(), T.Normalize(mean=(0.5,), std=(0.5,))
    ])

    val_transform = test_transform = T.Compose([
        T.ToTensor(), T.Grayscale(), T.Normalize(mean=(0.5,), std=(0.5,))
    ])

    train_set = CIFAR10(root='CIFAR10', train=True, transform=train_transform, download=False)

    train_set_length = int(0.8 * len(train_set))
    val_set_length = len(train_set) - train_set_length

    train_set, val_set = random_split(train_set, [train_set_length, val_set_length])
    test_set = CIFAR10(root='CIFAR10', train=False, transform=test_transform, download=False)
    train_loader = DataLoader(train_set, batch_size=b_size, shuffle=True)
    val_loader = DataLoader(val_set, batch_size=b_size)
    test_loader = DataLoader(test_set, batch_size=b_size)

    files = open("Test_Results.txt", "a")
    #variables: val, num_epochs, train_loader, val_loader, test_loader, loss_function, optimizer, files
    #Main LOOP for Layer 1, Only learning rates
    for i in range(3):
        files.write("The SETUP -> L.Rate: " + str(l_rate[i]) + " \n")
        print("The SETUP -> L.Rate: " + str(l_rate[i]) + " \n")
        main_1layer(epoch_count, train_loader, val_loader, test_loader, l_rate[i], files)

    #Main LOOP for 2 Layers,
    # Learning Rate -> 0.01, 0.001, 0.0001
    # Layer Neurons -> 512 - 768 - 1024
    # Activation Func -> Relu, Tanh, Sigmoid
    # Total of -> 27 Hyperparameters


    for i in range(3):
        for j in range(3):
            for k in range(3):
                files.write("The SETUP -> L.Rate: " + str(l_rate[j]) + " , H.Layer: " + str(layer_1[k])
                + " , Act. Func: " + activation_f[i] + " \n")
                print("The SETUP -> L.Rate: " + str(l_rate[j]) + " , H.Layer: " + str(layer_1[k])
                + " , Act. Func: " + activation_f[i] + " \n")
                main_2layer(activation_f[i], epoch_count, train_loader, val_loader, test_loader, l_rate[j], layer_1[k], files)

    #Main LOOP for 3 layers,       
    #Main LOOP for 2 Layers,
    # Learning Rate -> 0.01, 0.001, 0.0001
    # Layer Neurons -> 400, 200 - 512, 256 - 1024, 512
    # Activation Func -> Relu, Tanh, Sigmoid
    # Total of -> 27 Hyperparameters
    
    for i in range(3):
        for j in range(3):
            for k in range(3):
                files.write("The SETUP -> L.Rate: " + str(l_rate[j]) + " , H.Layer: " + str(layer_2[k][0])
                + "-" + str(layer_2[k][1]) + " , Act. Func: " + activation_f[i] + " \n")
                print("The SETUP -> L.Rate: " + str(l_rate[j]) + " , H.Layer: " + str(layer_2[k][0])
                + "-" + str(layer_2[k][1]) + " , Act. Func: " + activation_f[i] + " \n")
                main_3layer(activation_f[i], epoch_count, train_loader, val_loader, test_loader, l_rate[j], layer_2[k][0], layer_2[k][1], files)
    
    files.close()


#Results will be stored in Test_Results.txt
main_loop()
