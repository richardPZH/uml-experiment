function [ Entrance1 Entrance2 Entrance3 ] = testHierachical( imageVector , imageGist , labels , sratio, hierachin )
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

%need to Calculate the J(R,1) and store the buckets
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



% level 2 
% This training is different, This should train those bucket found in Level1 !!!
Entrance2 = cell( size( Entrance1 , 1 ) , 2 );
for m = 1 : size( Entrance1 , 1 )
	tmpGist = trGist( Entrance1{ m , 2} , : );
	tmplabels = trlabels( Entrance1{ m , 2 } );
    [W1 R1 centerPoint1] = Level2Hash(tmpGist,tmplabels,secondBit,'OURSITQ');

	% need to Calculate the J(R,1) and store the buckets
	XX = tmpGist - repmat( centerPoint1 , size( tmpGist , 1 ) , 1 );
	XX = XX * W1 * R1;
	XX( XX >= 0 ) = 1;
	XX( XX <  0 ) = 0;


	[ b i j ] = unique( XX , 'rows' );
	anoymousEntrance = cell( size( b , 1 ) , 2 );

	for n = 1 : size( b , 1 )
		anoymousEntrance{ n , 1 } = b( n , : );
		anoymousEntrance{ n , 2 } = Entrance1{ m , 2 }( find( j == n ) ); %Index of trGist
	end

	Entrance2{ m , 1 } = { W1 , R1 , centerPoint1 };
	Entrance2{ m , 2 } = anoymousEntrance;

end


% level 3 , at level 3 , we assume all the images are in the same class
% we split a little bit more, hope that return images will more like the input! 
%
% This implementation of Hierachical Hashing is bad bad bad!! Can We Do Better?
% Things are getting out of my control, dota it!
Entrance3 = cell( size( Entrance2 , 1 ) , 1 );

for m = 1 : size( Entrance2 , 1 )
	%L2Gist = trGist( Entrance1{ m , 1 } , : );  %no longer needed

	anoymousEntrance = Entrance2( m , 2 );

	L3cell = cell( size( anoymousEntrance , 1 ) , 1 );

	for n = 1 : size( anoymousEntrance , 1 )

		L3Gist = trGist( anoymousEntrance{ n , 2 } , : );

		[ W2 R2 centerPoint2 ] = Level3Hash( L3Gist , thirdBit , 'OURSITQ' );

		% need to Calculate the J(R,1) and store the buckets
		XX = L3Gist - repmat( centerPoint2 , size( L3Gist , 1 ) , 1 );
		XX = XX * W2 * R2;
		XX( XX >= 0 ) = 1;
		XX( XX <  0 ) = 0;


		[ b i j ] = unique( XX , 'rows' );

		tmpCell = cell( size( b , 1) , 2 );
		for o = 1 : size( b , 1)
			tmpCell{ o , 1 } = b( n , : );
			tmpCell{ o , 1 } = anoymousEntrance{ n , 2 }( find( j == o ) ); % think carefully this index the trGist and trlabel!
		end

		L3cell{ n , 1 } = tmpCell;


	end

	Entrance3{ m , 1 } = L3cell;


end



% Start the test 



% Plot the Accuracy-Recall curve































