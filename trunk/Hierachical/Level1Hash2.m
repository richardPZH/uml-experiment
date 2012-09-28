function [ W0 R0 centerPoint0 ] = Level1Hash2( X , labels , c , method )
% This function provide a second choice of the first level hashing
% Well, how many bits should we assign to the first level, 1bit, 2bit , 3?
%
% Input:
%      X , nx320 , the CIFAR dataset represent in the GIST row vector as in ITQ
%      labels, nx1 , range 0-9 , label information correspond to X, required when using CCA
%      c , how many bits assigned to the first level, we can optimize the whole Hierachical tree 
%      method, we provide a few method that can compare with each other :
%      'ITQ' , 'OURSITQ' , 'ITQRR' ...
%      
% Output:
%      W0, the CCA found projection matrix
%      R0, the ITQ like or ITQ found orthogonal transformation
%      centerPoint0, Because the ITQ related must centered the data first
%
%
% Author:
%     IMS@SCUT Once 2012/09/24

% X = double( X );

% find the center point
centerPoint0 = mean( X , 1 );

% make the data zero centered
X = X - repmat( centerPoint0 , size(X,1) , 1 );


% Convert the label information into the Y e {0,1} ( n x t );
t = max( labels ) + 1;
Y = zeros( size( labels , 1 ) , t );
for i = 1 : size( labels , 1 )
	Y( i , labels(i)+1 ) = 1;
end

% Apply the CCA dimension reduction, remember to read the cca.m
[ W0 , r ] = cca( X , Y , 0.0001 );

r = r( 1:c );
r = r';
W0 = W0( : , 1:c );            %for c bit code , get the leading colvector
r = repmat( r , size( W0 , 1 ) , 1 );
W0 = W0 .* r;                  % the W0 is scaled by its eigenvalue , from ITQ


switch method

case 'ITQ'
	% ITQ to find optimal rotation
	% default is 50 iterations
	% C is the output code
	% R is the roation found by ITQ

	% Project the n x 320 data into n x c data
	X = X * W0;

	[ C , R0 ] = ITQ( X , 50 );

case 'OURSITQ'
	% We propose the sensitive itq , we do improve its performance
	% default is 50 iterations
	% B is the embed data
	% R0 is the orthogonal transformation
	[ B , R0 ] = ITQSen( X , W0 , 50 ); 

case 'ITQRR'
	% Project the nx320 data into n x c data
	X = X * W0;
	R0 = randn( size( X , 2 ) , c );
    [ R0 U V ] = svd( R0 );
    R0 = R0( : , 1:c );

end













