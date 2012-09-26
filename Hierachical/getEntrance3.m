function [ Entrance ] = getEntrance3( E2 , trGist , bit , method )
%
% This function performs the third level hasing, its purpose is to hash the 
% hashed isolated class into more detail cluster. e.g.
% 		horse -> horse that running / standing / sitting
%       airplane -> sliding / flying
%
% We split a little bit more, hope that return images will more like the input! 
%
% Input:
%     E2, the second level Entrance , it is a cell( n , 2 )
%         the cell( x , 1) stores the [w1 r1 cp1]
%         the cell( x , 2) stores a cell( n , 2 )
%     trGist, the nx320 CIFAR , Gist represented images as a learning samples
% 	  trlabels, the nx1 CIFAR labels, 0~9
%     bit, how many bit is assigned to this hash code
%     method, e.g 'ITQ' 'OURSITQ' 'ITQRR'
%
%
% Output:
%     Entrance -> the third level Entrance , is is a cell( n ,1 )
%         the cell( x , 1 ) stores a L3cell -> cell( n , 2 )
%         L3cell( x , 1 ) stores w2 r2 cp2
%         L3cell( x , 2 ) stores a cell( n , 2 )
%         
%
%
% This implementation of Hierachical Hashing is bad bad bad!! Can We Do Better?
% Things are getting out of my control, dota it!


Entrance = cell( size( E2 , 1 ) , 1 );

for m = 1 : size( E2 , 1 )

	anoymousEntrance = E2( m , 2 );

	L3cell = cell( size( anoymousEntrance , 1 ) , 2 );

	for n = 1 : size( anoymousEntrance , 1 )

		L3Gist = trGist( anoymousEntrance{ n , 2 } , : );

		[ W2 R2 cP ] = Level3Hash( L3Gist , bit , method );

		% need to Calculate the J(R,1) and store the buckets
		XX = L3Gist - repmat( cP , size( L3Gist , 1 ) , 1 );
		XX = XX * W2 * R2;
		XX( XX >= 0 ) = 1;
		XX( XX <  0 ) = 0;


		[ b i j ] = unique( XX , 'rows' );

		tmpCell = cell( size( b , 1) , 2 );
		for o = 1 : size( b , 1)
			tmpCell{ o , 1 } = b( n , : );

			% think carefully this index the trGist and trlabel! why not find( j == 0 )
			tmpCell{ o , 2 } = anoymousEntrance{ n , 2 }(  j == o  ); 
		end

		
		L3cell{ n , 1 } = [ W2 , R2 , cP ];
		L3cell{ n , 2 } = tmpCell;


	end

	Entrance{ m , 1 } = L3cell;


end
