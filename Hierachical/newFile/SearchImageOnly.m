function [ ] = SearchImageOnly( sratio )
% Only use for search Image!

% This will create variable : gist label
load CIFAR10_GrayScale320_gist.mat
trGist = gist( 1:50000 , :);
trLabels = label( 1:50000 );

teGist = double( trGist );

%telabels = label( 50001 : end );
clear gist label %trGist trLabels 

% Get the random search images
R = randperm( size( teGist , 1 ) );
R = R( 1 : floor( 10000 * sratio ));

%Now we can use the trGist or tGist toget the test samples
teGist = teGist( R , : );
teVector = trLabels( R );
teLabels = trLabels( R );

% DEBUG TRICK
clear R;

% Start the test 
% teVector is the original samples that can be display to human
% teGist is the samples to be tested
% telabels is the ground true label

% load the EE1 file to get the E1
load 'EE1.mat'

global hiera
hiera = hierachin;

% Use the searchImage function to find the recall-precision; hierachical to search
[ r p ] = searchImage3( teGist , teLabels , teVector ,  trGist , trVector ,trLabels , E1 );

[ r , p ] = avgRPPlot( r , p , 0.05 );

% Plot average precision for top 500 retrieved images
figure( 1 );
precision500( r , p );

% Plot the Accuracy-Recall curve
figure( 2 );
title( 'recall-precision' );
plot( r , p , '-o' );
axis( [0 1 0 1] );
grid on;
hold on;