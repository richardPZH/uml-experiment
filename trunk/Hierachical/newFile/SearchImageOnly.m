function [ ] = SearchImageOnly( sratio )
% Only use for search Image!

% This will create variable : gist label
load CIFAR10_GrayScale320_gist.mat
trGist = gist( 1:50000 , :);
trLabels = label( 1:50000 );
teGist = gist( 50001 : end , : );

teGist = double( teGist );
%telabels = label( 50001 : end );
clear gist label trGist trLabels 

% load the test vector , this will create variable : batch_label data labels
load cifar-10-batches-mat\test_batch.mat
teVector = data;
teLabels = labels;
clear batch_label labels data

% Get the random search images
R = randperm( size( teGist , 1 ) );
R = R( 1 : floor(size( teGist , 1 ) * sratio ));

teGist = teGist( R , : );
teVector = teVector( R , :);
teLabels = teLabels( R );

% DEBUG TRICK
clear R;

% Start the test 
% teVector is the original samples that can be display to human
% teGist is the samples to be tested
% telabels is the ground true label

% load the EE1 file to get the E1
load 'EE1.mat'

% Use the searchImage function to find the recall-precision; hierachical to search
[ r p ] = searchImage3( teGist , teLabels , teVector ,  trGist , trVector ,trLabels , E1 );

[ r , p ] = avgRPPlot( r , p , 0.05 );

% Plot average precision for top 500 retrieved images
precision500( r , p );

% Plot the Accuracy-Recall curve
plot( r , p , '-o' );
axis( [0 1 0 1] );
grid on;
hold on;