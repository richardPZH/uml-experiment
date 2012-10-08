function [ E1 E2 E3 ] = testHierachical( imageVector , imageGist , labels , sratio, hierachin )
% embed all the three level hash and perform:
% 	1.the recall-accuracy plot
% 	2.the precision plot
%   3.may plot the original image, show to human
%
% This may be a buggy file
%
% Input :
%     imageVector, the nx3072 CIFAR original, use for plotting image
%     imageGist , the nx320 GIST representation of CIFAR
%     labels , the true class of the image , range from 0-9 (CIFAR)
%     sratio, the ratio of search/total , 0.05 - 0.3 is recommened
%     hierachin, usually a 3x1 vector, 
%     	hierachin(1) is the bits of the first level
%		hierachin(2) is the bits of the second level
%  		hierachin(3) is the bits of the third level
%
% Output:
%
%
% Author: 
%     IMS@SCUT Once 2012/09/24
%     

% split the hierachical bit sequence,( firstBit secondBit thirdBit  ) >= 1 is require
firstBit = hierachin(1);
secondBit = hierachin(2);
thirdBit = hierachin(3);

if ( firstBit <= 0 || secondBit <= 0 || thirdBit <= 0 )
	disp('testHierachical: Warning! Input Parameter Is Not Set Properly..' );
	return ; 
end

% split into training Image and search Image
R = randperm( size( labels , 1 ) );
num_search = floor ( size( labels , 1 ) * sratio ) ;  %floor or ceil is needed
num_train  = size( labels , 1 ) - num_search ;

trGist = imageGist( R(1:num_train ) , : );
trVector = imageVector( R(1:num_train ) , : ); 
trlabels = labels( R( 1:num_train ) );

R( 1:num_train ) = [];

teGist = imageGist( R , :  );
teVector = imageVector( R , : );
telabels = labels( R );

trGist = double( trGist );
teGist = double( teGist );
% Strat the hash

% level 1
[ E1 ] = getEntrance1( trGist , trlabels , firstBit , 'ITQ' );


% level 2 
E2 = getEntrance2( E1 , trGist , trlabels , secondBit , 'ITQ' );


% level 3 , at level 3 , we assume all the images are in the same class
E3 = getEntrance3( E2 , trGist , thirdBit , 'ITQ' );


% Start the test 
% teVector is the original samples that can be display to human
% teGist is the samples to be tested
% telabels is the ground true label

[ r p ] = searchImage( teGist , telabels , teVector ,  trGist , trlabels , trVector , E1 , E2 , E3 );

	






% Plot the Accuracy-Recall curve

plot( r , p , 'x' );





























