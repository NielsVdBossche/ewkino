from re import M
from keras.models import Sequential, Model
from keras.layers.core import Dense, Activation, Flatten
import keras.layers as layers
import keras.layers.convolutional as conv
from keras.layers.merge import concatenate
from keras.layers.pooling import MaxPooling2D
from keras.regularizers import l2
from keras.optimizers import SGD
from keras.utils import plot_model
from keras.backend import slice

import numpy as np

def altKerasModel():
    # input layer
    inputLayer = layers.Input(shape=(10,4))
    
    split_1 = layers.Lambda(lambda x: slice(x, (0, 0), (6, -1)))(inputLayer)
    split_2 = layers.Lambda(lambda x: slice(x, (7, 0), (-1, -1)))(inputLayer)

    d1 = conv.Conv2D(32, kernel_size=(2,2), activation='relu')(split_1)
    #d1 = conv.Conv2D(16, kernel_size=2, activation='relu')(d1)
    #d1 = conv.Conv2D(16, kernel_size=2, activation='relu')(d1)
    d1 = MaxPooling2D(pool_size=(2,2))(d1)
    d1 = Flatten()(d1)

    d2 = conv.Conv2D(32, kernel_size=(2,2), activation='relu')(split_2)
    d2 = MaxPooling2D(pool_size=(2,2))(d2)
    d2 = Flatten()(d2)

    merge = concatenate([d1, d2])

    dm = Dense(128, activation='relu')(merge)
    out = Dense(3, activation='softmax')(dm)
    #d2 = conv.Conv2D(16, kernel_size=2, activation='relu')(d2)
    #d2 = conv.Conv2D(16, kernel_size=2, activation='relu')(d2)
    model = Model(inputs=inputLayer, outputs=out)
    model.save("Models/SplitInMiddle.h5")
    print(model.summary())
    plot_model(model, to_file='shared_input_layer.png')


def generate_model(num_hidden_layers, nodes_hidden_layer, num_input_nodes=30, num_output_nodes=3):
    name = "keras_" + str(num_input_nodes) + "_" + str(num_output_nodes) + "_" + str(num_hidden_layers) + "_" + str(nodes_hidden_layer)

    l2_val = 1e-5
    
    model = Sequential()
    
    # Hidden layer 1
    # NOTE: Number of input nodes need to be defined in this layer
    model.add(Dense(nodes_hidden_layer, activation='relu', input_dim=num_input_nodes))
    
    # Hidden layer 2 to num_hidden_layers
    # NOTE: Here, you can do what you want
    for k in range(num_hidden_layers-1):
        model.add(Dense(nodes_hidden_layer, activation='relu'))
    
    # Ouput layer
    # NOTE: Use following output types for the different tasks
    # Binary classification: 2 output nodes with 'softmax' activation
    # Regression: 1 output with any activation ('linear' recommended)
    # Multiclass classification: (number of classes) output nodes with 'softmax' activation
    model.add(Dense(num_output_nodes, activation='softmax'))
    
    # Compile model
    # NOTE: Use following settings for the different tasks
    # Any classification: 'categorical_crossentropy' is recommended loss function
    # Regression: 'mean_squared_error' is recommended loss function
    opt = SGD(lr=0.05, momentum=0.4)

    model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['categorical_accuracy',])
    
    # Save model
    model.save("Models/" + name + ".h5")
    
    # Additional information about the model
    # NOTE: This is not needed to run the model
    
    # Print summary
    model.summary()
    
    # Visualize model as graph
    try:
        plot_model(model, to_file=name+".png", show_shapes=True)
    except:
        print('[INFO] Failed to make model plot')

if __name__ == "__main__":
    # altKerasModel()
    num_hidden_layers = [3, 4]
    nodes_hidden_layer = [256]
    num_input_nodes = [46, 49]
    # num_output_nodes = np.linspace()
    for num_input in num_input_nodes: 
        for num_hid in num_hidden_layers: 
            for nodes in nodes_hidden_layer:
                generate_model(num_hid, nodes, num_input)
