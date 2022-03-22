from keras.models import Sequential
from keras.layers.core import Dense, Activation
from keras.regularizers import l2
from keras.optimizers import SGD
from keras.utils import plot_model

import numpy as np

def generate_model(num_hidden_layers, nodes_hidden_layer, num_input_nodes=30, num_output_nodes=3):
    name = "keras_" + str(num_input_nodes) + "_" + str(num_output_nodes) + "_" + str(num_hidden_layers) + "_" + str(nodes_hidden_layer)

    l2_val = 1e-5
    
    model = Sequential()
    
    # Hidden layer 1
    # NOTE: Number of input nodes need to be defined in this layer
    model.add(Dense(nodes_hidden_layer, activation='relu', W_regularizer=l2(l2_val), input_dim=num_input_nodes))
    
    # Hidden layer 2 to num_hidden_layers
    # NOTE: Here, you can do what you want
    for k in range(num_hidden_layers-1):
        model.add(Dense(nodes_hidden_layer, activation='relu', W_regularizer=l2(l2_val)))
    
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
    model.compile(loss='categorical_crossentropy', optimizer=SGD(lr=0.01), metrics=['categorical_accuracy',])
    
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
    num_hidden_layers = np.arange(1, 3, 1)
    nodes_hidden_layer = [128, 256, 512]
    num_input_nodes = [35, 36]
    # num_output_nodes = np.linspace()
    for num_input in num_input_nodes: 
        for num_hid in num_hidden_layers: 
            for nodes in nodes_hidden_layer:
                generate_model(num_hid, nodes, num_input)
