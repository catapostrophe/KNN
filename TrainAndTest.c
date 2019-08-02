//
//  TrainAndTest.c
//  MLCoursework
//
//  This is a fairly inefficient implentation that does not use any dynamic memory allocation
// because that wouldnot be safe on the DEWIS marking system
//
//  Created by Jim Smith on 06/02/2017.
//  Copyright © 2017 Jim SmithJim Smith. All rights reserved.
//

#include "TrainAndTest.h"
#include <math.h>



//declare this array as static but make it available to any function in this file
//in case we want to store the training examples  and use them later
static    double myModel[NUM_TRAINING_SAMPLES][NUM_FEATURES];
//even if each item in the training set is from a diffferent class we know how many there are
static char myModelLabels[NUM_TRAINING_SAMPLES];

static int labelsUsed[255];

static int trainingSetSize=0;

double calculate_distance();

int  train( double **trainingSamples, char *trainingLabels, int numSamples, int numFeatures)
{
    int returnval = 1;
    int sample, feature;
    
  
    
    //clean the model because C leaves whatever is in the memory
    for (sample=0; sample < NUM_TRAINING_SAMPLES; sample++) {
        for (feature=0; feature<NUM_FEATURES; feature++) {
            myModel[sample][feature] = 0.0;
        }
    }
    
    //sanity checking
    if ( numFeatures > NUM_FEATURES || numSamples > NUM_TRAINING_SAMPLES) {
        fprintf(stdout,"error: called train with data set larger than spaced allocated to store it");
        returnval=0;
    }
    
    //this is a silly trivial train()_ function
    
    
    
    //make a simple copy of the data we are being passed but don't do anything with it
    //I'm just giving you this for the sake of people less familiar with pointers etc.
    
     
     if(returnval==1) {
        //store the labels and the feature values
        trainingSetSize = numSamples;
        int index,feature;
        for (index=0; index < numSamples; index++) {
            myModelLabels[index] = trainingLabels[index];
            for (feature=0; feature < numFeatures; feature++) {
                myModel[index][feature] = trainingSamples[index][feature];
            }
        }
        fprintf(stdout,"data stored locally \n");
    }//end else
    
    
    //now you could do whatever you like with the data
    //for example,  you could populate some rules etc.
    //you were given pseudocode in semester 1 to do this
    // you could also normalise the data to remove scaling effects if you want to use something like a MLP or kNN
    //just remember that anything that you want to acess in your predictLabel() function
    //needs to be declared static at the top of this file - as I have done for the "myModel"  and myModelLabels data .
    
    
    
    return returnval;
}
// Euclidean distance calculator
double calculate_distance(double *sample, double *pinkfluffyunicorn) {

    double diff;
    double dist = 0.00;
    
    for (int s = 0; s < NUM_FEATURES ; s++) {
        diff = sample[s] - pinkfluffyunicorn[s];
        dist += diff*diff; 
    }
    
   return sqrt(dist); 
}
   
char  predictLabel(double *sample, int numFeatures){

    for (int n = 0; n < 256; n++) {
       labelsUsed[n] = 0; 
    }
    //jhwgdjhwgedh NO!
    //Lets get this party started
    double distances[NUM_TRAINING_SAMPLES];
    int sorted_indexes[NUM_TRAINING_SAMPLES];
    
    for (int model = 0; model < NUM_TRAINING_SAMPLES; model++) {
        distances[model] = calculate_distance(sample, myModel[model]);
        sorted_indexes[model] = model;
    }
  
    // Takes a value in the array and compares it against the value before it to 
    // sort distances in ascending order
    
    for(int i=0; i < NUM_TRAINING_SAMPLES; i++) {
        for (int j=0; j < (NUM_TRAINING_SAMPLES-1); j++) {
            if(distances[j] > distances[j+1]) {
                double temp = distances[j];
                distances[j] = distances[j+1];
                distances[j+1] = temp;
                
                int temp_sort = sorted_indexes[j];
                sorted_indexes[j] = sorted_indexes[j+1];
                sorted_indexes[j+1] = temp_sort;    
            }
        }
    }
    
    // Clears the array so we don't get errors
    for (int clear = 0; clear < 256; clear++) {
       labelsUsed[clear] = 0; 
    }
   
    int k = 7; //Odd number so as to not have a tied answer
   
    // Assigns the distance and character value so we can get the 
    // k nearest neighbours
     for(int n = 0; n < k; n++) {
        char class = myModelLabels[sorted_indexes[n]];
        labelsUsed[(int)class]++;
    }

    int current_max = 0;
    char prediction;
    
    // Check each label's score
    for(int i = 0; i < 256; i++) {
        if (current_max < labelsUsed[i]) {
            current_max = labelsUsed[i];
            prediction = (char) i;
        }
    }
    
    return prediction;
}
