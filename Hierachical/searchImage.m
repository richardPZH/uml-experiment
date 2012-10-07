function [ recall precision ] = searchImage( inGist , inLabel , inVector , trGist , trLabels , trVector , E1 , E2 , E3 )
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


precision = 1;
recall = 0;

W0=  E1{ 1 , 1 }{ 1 };
R0 = E1{ 1 , 1 }{ 2 };
cP0 = E1{ 1 , 1 }{ 3 };

% first level hash
XX = inGist - repmat( cP0 , size( inGist , 1 ) , 1 );

XX = XX * W0 * R0;

XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;


for a = 1 : 1 : size( XX , 1 )   %step is one, incase it goes downward, we have size( XX,1 ) samples to query

	sample = inGist( a , : );       %the sample is the query sample..
	code = XX( a , : );             %this is the sample first level hash code
	sampleLabel  = inLabel( a );    %sample true label

	totalReturnImages = 0 ;
	correctImages = 0 ;
	databaseImages = sum( trLabels == sampleLabel ); 

	% In the Entrace 1 , E1{1,2} is the binary code matrix, one row, one point
	L1Code = E1{ 1 , 2 };

 	hmd1 = CalHammingDist( code , L1Code , 'vec' );
	minDist1 = min( hmd1 );
	maxDist1 = max( hmd1 );

	in2E2 = find( hmd1 == minDist1 );

	for b = 1 : 1 : length( in2E2 )  %careful for this for loop, not going downward
		
        ch = E2{ in2E2( b ) , 1 };
        W  = ch{ 1 };
        R  = ch{ 2 };
        cP = ch{ 3 };

		sc = sample - cP;      %center the point
		sc = sample * W * R ;  %project and rotate
		sc( sc >= 0 ) = 1;
		sc( sc <  0 ) = 0;
		

		L2Code = E2{ in2E2( b ) , 2 };
		%E2anoymous = E2{ in2E2( b ) , 3 }; 

		hmd2 = CalHammingDist( sc , L2Code , 'vec' ); 

		minDist2 = min( hmd2 );
		maxDist2 = max( hmd2 );

        L3cell = E3{ in2E2( b ) , 1 }; %Since the second levle is known, the third one is known too.
        
		for c = minDist2 : 1 : maxDist2

			in2E3 = find( hmd2 == c );

			for d = 1 : 1 : length( in2E3 )

                W  = L3cell{ in2E3( d ) , 1 }{ 1 };
                R  = L3cell{ in2E3( d ) , 1 }{ 2 };
                cP = L3cell{ in2E3( d ) , 1 }{ 3 };
                
				L3Code = L3cell{ in2E3( d ) , 2 };
				sindex = L3cell{ in2E3( d ) , 3 };

				sc = sample - cP;
				sc = sample * W * R;
				sc( sc >= 0 ) = 1;
				sc( sc <  0 ) = 0;

				hmd3 = CalHammingDist( sc , L3Code , 'vec' );

				minDist3 = min( hmd3 );
				maxDist3 = max( hmd3 );

				for e = minDist3 : maxDist3

					findLabels = find( hmd3 == e );

					for f = 1 : length( findLabels )

						rtindex = sindex{ findLabels( f ) };

						totalReturnImages = totalReturnImages + length( rtindex );
						correctImages = correctImages + sum( trLabels( rtindex ) == sampleLabel );

					end

					precision = [ precision , ( correctImages / ( totalReturnImages + eps )) ];
					recall = [ recall , ( correctImages / databaseImages ) ];

                end	
            end
        end
	end
end












