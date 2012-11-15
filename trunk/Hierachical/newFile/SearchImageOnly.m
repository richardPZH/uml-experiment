function [ ] = SearchImageOnly( )
% Only use for search Image!

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
[ r p ] = searchImage3( teGist , teLabels , teVector , trLabels , E1 );

[ r , p ] = avgRPPlot( r , p , 0.05 );

% Plot the Accuracy-Recall curve
plot( r , p , '-o' );
axis( [0 1 0 1] );
grid on;
hold on;