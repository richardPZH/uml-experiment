function [ W2 R2 centerPoint2 ] = Level3Hash( X , c , method )
% This is the Level 3 Hashing function
% 
% Input:
%      X, the nx320 GIST image representation that hash into the same bucket in Level 2
%      c, how many bits to encode level 3 images
%      method, which way to find the R2 rotation
%
% Output:
%      W2, the PCA found projection matrix
%      R2, the ITQ like found orthonal rotation matrix
%      centerPoint2, the centerPoint of these X 
%
% Author:
%      IMS@SCUT Once 2012/09/23
%

X = double( X );

% center the data, important to use the PCA and ITQ like
centerPoint2 = mean( X );
X = X - repmat( centerPoint2 , size( X , 1 ) , 1 );

% Perform the PCA
[W2 , l ] = eigs( cov(X) , c );


switch ( method )

case 'ITQ'
	% Project the data, itq default iteration is 50
	X = X * W2;
	[ Y , R2] = ITQ( X , 50 );
	

case 'ITQRR'
	% Project the data, itq default iteration is 50
	X = X * W2;

	R2 = randn( size( X , 2 ) , c );
	[R2 S V ] = svd( R2 );
	R2 = R2( : , 1:c );


case 'OURSITQ'
	% ITQ Sensitivity , default iteration 50 is quite enough
	[ B R2 ] = ITQSen( X , W2 , 50 );

end


