function [ Entrance2 ] = getEntrance2( Entrance1 , trGist , trlabels , secondBit , method )
%
%
%
%
% This training is different, This should train those bucket found in Level1 !!!

Entrance2 = cell( size( Entrance1 , 1 ) , 2 );

for m = 1 : size( Entrance1 , 1 )
	tmpGist = trGist( Entrance1{ m , 2} , : );
	tmplabels = trlabels( Entrance1{ m , 2 } );
    [W1 R1 centerPoint1] = Level2Hash(tmpGist,tmplabels,secondBit,method);

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
