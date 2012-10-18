function [ ] = testHierachical2( trVector , trGist , trlabels , teVector , teGist , telabels , hierachin )

% embed all the three level hash and perform:
% 	1.the recall-precision plot
%   2.may plot the original image, show to human
%
% Input :
%     trVector, the nx3072 CIFAR original training image , use for plotting image
%     trGist, the nx320 GIST representation of CIFAR training image;
%     trlabels, the true class of the image , range from 0-9 (CIFAR) training
%     teVector, the nx3072 CIFAR original testing image
%     teGist,  the nx320 Gist representation of CIFAR testing image;
%     telabels, the true class of the image , range from 0-9 (CIFAR) testing
%     hierachin, usually a 3x1 vector, 
%     	hierachin(1) is the bits of the first level
%		hierachin(2) is the bits of the second level
%  		hierachin(3) is the bits of the third level
%
% Output:
%
%
% Author: 
%     IMS@SCUT Once 2012/10/18
%     

% split the hierachical bit sequence,( firstBit secondBit thirdBit  ) >= 1 is require
firstBit = hierachin(1);
secondBit = hierachin(2);
thirdBit = hierachin(3);

if ( firstBit <= 0 || secondBit <= 0 || thirdBit <= 0 )
	disp('testHierachical: Warning! Input Parameter Is Not Set Properly..' );
	return ; 
end

% Strat the hash

% level 1
E1 = getEntrance1( trGist , trlabels , firstBit , 'ITQ' );


% level 2 
E2 = getEntrance2( E1 , trGist , trlabels , secondBit , 'ITQ' );


% level 3 , at level 3 , we assume all the images are in the same class
E3 = getEntrance3( E2 , trGist , thirdBit , 'ITQ' );

% E1 , E2 , their index to the trGist is no longer useful, delete them
% But E3 is useful for precision-recall calculation
%E1( 1 , 3 ) = [];
%E2( : , 3 ) = [];


% Start the test 
% teVector is the original samples that can be display to human
% teGist is the samples to be tested
% telabels is the ground true label

[ r , p ] = searchImage( teGist , telabels , teVector ,  trGist , trlabels , trVector , E1 , E2 , E3 );

[ r , p ] = avgRPPlot( r , p , 0.05 );

% Plot the Accuracy-Recall curve
plot( r , p , '-o' );
axis( [0 1 0 1] );
grid on;
hold on;



