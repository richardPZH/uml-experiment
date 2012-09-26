function [ Entrance ] = getEntrance2( E1 , trGist , trlabels , bit , method )
%
% This function performs the second hashing, its purpose is to hash the hashed 
% general class into isolated classes. e.g.
%      living things -> bird cat deer dog horse frog
%      things on in the air -> bird airplane 
%
% Input:
%     E1, the first level Entrance , it is a cell( n , 2 ) , cell( x , 1 ) stores
%         the hashcode, cell( x , 2 ) stores index of the trGist
%     trGist, the nx320 CIFAR , Gist represented images as a learning samples
% 	  trlabels, the nx1 CIFAR labels, 0~9
%     bit, how many bit is assigned to this hash code
%     method, e.g 'ITQ' 'OURSITQ' 'ITQRR'
%
%
% Output:
%     Entrance -> the second level Entrance, it is a cell( n , 2 ) , 
%         cell( x , 1 ) stores the w1 r1 cp1 of the E1{ x , : }
%         cell( x , 2 ) stores a cell( n , 2 )
%

Entrance = cell( size( E1 , 1 ) , 2 );

for m = 1 : size( E1 , 1 )

	tmpGist = trGist( E1{ m , 2} , : );

	tmplabels = trlabels( E1{ m , 2 } );

    [ W1 R1 cP ] = Level2Hash( tmpGist , tmplabels , bit , method );

	% need to Calculate the J(R,1) and store the buckets
	XX = tmpGist - repmat( cP , size( tmpGist , 1 ) , 1 );
	XX = XX * W1 * R1;
	XX( XX >= 0 ) = 1;
	XX( XX <  0 ) = 0;


	[ b i j ] = unique( XX , 'rows' );
	anoymousEntrance = cell( size( b , 1 ) , 2 );

	for n = 1 : size( b , 1 )
		anoymousEntrance{ n , 1 } = b( n , : );

		%Index of trGist % This code I used find and matlab ask me to use
		%the logical expression j == n to avoid vector generation?? note!
		anoymousEntrance{ n , 2 } = E1{ m , 2 }(  j == n  ); 
	end

	Entrance{ m , 1 } = [ W1 , R1 , cP ];
	Entrance{ m , 2 } = anoymousEntrance;

end
