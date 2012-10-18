function [ ]  = loadFileAndCal( )

% This file load the CIFAR dataset and preprocess 
%
% 
% Author:
%      IMS@SCUT Once

clear all

% This will create variable : gist label
load CIFAR10_GrayScale320_gist.mat
trGist = gist( 1:50000 , :);
trLabels = label( 1:50000 );
teGist = gist( 50001 : end , : );
telabels = label( 50000:end );
clear gist label

% This well create variable : batch_label data labels
load cifar-10-batches-mat\data_batch_1.mat
trVector = data;
clear batch_label labels data

load cifar-10-batches-mat\data_batch_2.mat
trVector = [ trVector ; data ];
clear batch_label labels data

load cifar-10-batches-mat\data_batch_3.mat
trVector = [ trVector ; data ];
clear batch_label labels data

load cifar-10-batches-mat\data_batch_4.mat
trVector = [ trVector ; data ];
clear batch_label labels data

load cifar-10-batches-mat\data_batch_5.mat
trVector = [ trVector ; data ];
clear batch_label labels data

% load the test vector , this will create variable : batch_label data labels
load cifar-10-batches-mat\test_batch.mat
teVector = data;
teLabels = labels;
clear batch_label labels data












