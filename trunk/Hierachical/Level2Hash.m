function [ W1 R centerPoint ] = Level2Hash( X , labels , c , method )
% Function Level2Hash perform the second level hashing
% In this level, we may have to hash many classes instead of 0 and 1 in 1st level
% Label Information is need to perform the CCA Dimension Reduction
%
% Input:
%      X , nx320 , the CIFAR dataset represent in the GIST row vector as in ITQ
%      labels, nx1 , range 0-9 , label information correspond to X, required when using CCA
%      c , how many bits assigned to the second level, we can go through 2D 3D and can calculate how many bits we need to classify those 
%      method, we provide a few method that can compare with each other :
%      'ITQ' , 'OURSITQ' , 'ITQRR' ...
%      
%      
% Output:
%      W1, the CCA found projection matrix
%      R, the OURSITQ or ITQ found orthogonal transformation
%      centerPoint, Because the ITQ related must centered the data first
%
% Author:
%      IMS@SCUT 2012/09/23

% This is a bad habit, but we need to handle it because the CIFAR dataset
% X = double( X );  

% find the center point 
centerPoint = mean( X , 2 );

% make the data zero centered
X = X - repmat( centerPoint , size( X , 1 ) , 1 );

% we don't need to normalize the data because every dimension of the image is equal, I assumed


% Convert the label information into the Y e {0,1} ( n x t );
t = max( labels ) + 1;
Y = zeros( size( labels , 1 ) , t );
for i = 1 : size( labels , 1 )
	Y( i , labels(i)+1 ) = 1;
end

% Apply the CCA dimension reduction, remember to read the cca.m
[ W1 , r ] = cca( X , Y , 0.0001 );

r = r( 1:c );
r = r';
W1 = W1( : , 1:c );            %for c bit code , get the leading colvector
r = repmat( r , size( W1 , 1 ) , 1 );
W1 = W1 .* r;                  % the W1 is scaled by its eigenvalue , from ITQ


switch method

case 'ITQ'
	% ITQ to find optimal rotation
	% default is 50 iterations
	% C is the output code
	% R is the roation found by ITQ

	% Project the n x 320 data into n x c data
	X = X * W1;

	[ C , R ] = ITQ( X , 50 );

case 'OURSITQ'
	% We propose the sensitive itq , we do improve its performance
	% default is 50 iterations
	% B is the embed data
	% R is the orthogonal transformation
	[ B , R ] = ITQSen( X , W1 , 50 ); 

case 'ITQRR'
	% Project the nx320 data into n x c data
	X = X * W1;
	R = randn( size( X , 2 ) , c );
    [ R U V ] = svd( R );
    R = R( : , 1:c );

end






























