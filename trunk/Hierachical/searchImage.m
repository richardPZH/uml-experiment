function [ recall precision ] = searchImage( inGist , inLabel , inVector , trGist , trLabels , trVector , W0 , R0 , cP0 , E1 , E2 , E3 )
%
% This function use the testing image to evaluate the performance 
% of the Hierachical hashing method... 
%
% Input:
%     inGist, nx320 CIFAR Gist image representation, n images to be evaluate
%	  inLabel, nx1, ground true label of the image; 
%     inVector, the nx3072 original image, can use to display images to human
%     trGist, mx320 training Gist image representation
%     trLabels, 
%     trVector,
%     W0, R0 , cP0 , first level projection , orthogonal rotation , centerPoint
%     E1, the first level entrance
%     E2, the second level entrance
%     E3, the third level entrance
%
% Output:
%     recall: average recall of all the n images
%     precision: average precision of ...
%
% Authors:
%     IMS@SCUT Once 
% Advices:
%     How to use more vector operation rather than for loop


% first level hash
XX = inGist - repmat( cP0 , size( inGist , 1 ) , 1 );

XX = XX * W0 * R0;

XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;


for a = 1 : size( XX , 1 )

	sample = trGist( a , : );

	code = XX( a , : );
	label  = inLabel( a );


	% In the Entrace 1 , E1{1,1} is the binary code matrix, one row, one point
	L1Code = E1{ 1 , 1 };

	hmd = CalHammingDist( code , L1Code , 'vec' );
	minDist1 = min( hmd );
	maxDist1 = max( hmd );

	in2E2 = find( hmd == minDist1 );

	for b = 1 : length( in2E2 )
		
		[ W , R , cP ] = E2{ in2E2( b ) , 1 };

		sc = sample * W * R ;
		sc( sc >= 0 ) = 1;
		sc( sc <  0 ) = 0;
		

		L2Code = E2{ in2E2( b ) , 2 };

		hmd = CalHammingDist( sc , L2Code , 'vec' ); 

		minDist2 = min( hmd );
		maxDist2 = max( hmd );




	end


















