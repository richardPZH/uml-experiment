function [ ] = testHierachical( imageVector , imageGist , labels , sratio, hierachin )
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

% split the hierachical bit sequence, firstBit secondBit thirdBit >= 1 is require
firstBit = hierachin(1);
secondBit = hierachin(2);
thirdBit = hierachin(3);

% split into training Image and search Image
R = randperm( size( labels , 1 ) );
num_search = floor ( size( labels , 1 ) * sratio );  %floor or ceil is needed
num_train  = size(labels , 1 ) - num_search;

trGist = imageGist( R(1:num_train ) , : );
trVector = imageVector( R(1:num_train ) , : ); 
trlabels = labels( R(1:num_train ) );

R( 1:num_train ) = [];

teGist = imageGist( R , :  );
teVector = imageVector( R , : );
telabels = labels( R );

% Strat the hash

% level 1
[W0 R0 centerPoint0] = Level1Hash2( trGist , trlabels , firstBit , 'OURSITQ' );

%Calculate the J(R,1) and store the buckets
XX = trGist - repmat( centerPoint0 , size( trGist , 1 ) , 1 );
XX = XX * W0 * R0;
XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

[ b i j ] = unique( XX , 'rows' );
Entrance1 = cell( size( b , 1 ) , 2 );

for m = 1 : size( b , 1 )
	Entrance1{ m , 1 } = b( m , :);
	Entrance1{ m , 2 } = find( j == m );
end



% level 2 %This traing is different, This should train those bucket found in Level1 !!!

% level 3





% Start the test 



% Plot the Accuracy-Recall curve































