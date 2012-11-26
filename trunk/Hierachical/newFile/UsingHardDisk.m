function [ ]  = UsingHardDisk( sratio , hierachin )

% This file first load the CIFAR dataset and preprocess, then run the
% hierachical hashing. Because We may run out of memory we need to use the 
% hard disk
%
% Input:
%      hierachin, the three level hashing [ a b c ] a for the first level ,
%          b for second level , c for the third level
%      sratio, the ratio of search/total , 0.05 - 0.3 is recommened
% Author:
%      IMS@SCUT Once
%      2012/11/15

% Used to search
global hiera
hiera = hierachin

% This will create variable : gist label
load CIFAR10_GrayScale320_gist.mat
trGist = gist( 1:50000 , :);
trLabels = label( 1:50000 );
teGist = gist( 50001 : end , : );
%telabels = label( 50001 : end );
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

% split the hierachical bit sequence,( firstBit secondBit thirdBit  ) >= 1 is require
firstBit = hierachin(1);
secondBit = hierachin(2);
thirdBit = hierachin(3);

if ( firstBit <= 0 || secondBit <= 0 || thirdBit <= 0 )
	disp('testHierachical: Warning! Input Parameter Is Not Set Properly..' );
	return ; 
end

trGist = double( trGist );
teGist = double( teGist );

%Now we have all the data , start the level 1 hashing
E1 = getEntrance1( trGist , trLabels , firstBit , 'ITQ' );

% Build the structure in the hard disk 
buildStructure( trGist , trLabels , secondBit , thirdBit , E1 , 'ITQ' );
% end building structure

% clear E1 {1,3} useless information 
E1{ 1 , 3 } = [];

save 'EE1' E1 trLabels trVector trGist hierachin

% Get the random search images
R = randperm( size( teGist , 1 ) );
R = R( 1 : floor(size( teGist , 1 ) * sratio ));
teGist = trGist( R , : );
teVector = trVector( R , :);
teLabels = trLabels( R );

% DEBUG TRICK
% teGist = [ trGist( 1 , : ) ; teGist ];
% 
% teVector = [ trVector( 1 , : ) ; teVector ];
% teLabels = [ trLabels( 1 ) ; teLabels ];

% DEBUG TRICK
clear R;

% Start the test 
% teVector is the original samples that can be display to human
% teGist is the samples to be tested
% telabels is the ground true label

% Use the searchImage function to find the recall-precision; hierachical to search
[ r p ] = searchImage3( teGist , teLabels , teVector , trGist , trVector ,trLabels , E1 );

[ r , p ] = avgRPPlot( r , p , 0.05 );

% Plot average precision for top 500 retrieved images
figure( 1 );
title( 'precision@500' );
precision500( r , p );

% Plot the Accuracy-Recall curve
figure( 2 );
title( 'recall-precision' );
plot( r , p , '-o' );
axis( [0 1 0 1] );
grid on;
hold on;












