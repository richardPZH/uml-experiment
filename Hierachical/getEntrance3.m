function [ Entrance3 ] = getEntrance3( Entrance2 , trGist , thirdBit , method )
%
%
%
%


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

		[ W2 R2 centerPoint2 ] = Level3Hash( L3Gist , thirdBit , method );

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
