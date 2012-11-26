function [ r p ] = searchImage3( inGist , inLabel ,  inVector , trGist , trVector ,trLabels , E1 )
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
%     IMS@SCUT Once 2012/11/04
%
% Advices:
%     How to use more vector operation rather than for loop

global numOfRet
global numOfHit
global numOfSam
global recall
global precision

r = 0;
p = 1;

% the hamming distance in level 1, which we need to perdefine, but how to
% find a good L1Dis??
global hiera
L1Dis = floor( hiera(1) * 0.8 );

%get the first level hash code
W0=  E1{ 1 , 1 }{ 1 };
R0 = E1{ 1 , 1 }{ 2 };
cP0 = E1{ 1 , 1 }{ 3 };

% first level hash
XX = inGist - repmat( cP0 , size( inGist , 1 ) , 1 );

XX = XX * W0 * R0;

XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

% In the Entrace 1 , E1{1,2} is the binary code matrix, one row, one group images
L1Code = E1{ 1 , 2 };

for numOfImage = 1 : size( inLabel , 1 )
   
    imageGist = inGist( numOfImage , : );     %search image in gist
    imageL1Code = XX( numOfImage , : );       %search image 1st code
    imageVector = inVector( numOfImage , : ); %search image in RGB
    imageLabel = inLabel( numOfImage );       %search image label
    
    numOfSam = sum( trLabels == imageLabel );
    numOfRet = eps;
    numOfHit = 0;
    
    recall = 0;
    precision = 1;
    
    %search one image starts
    hmd = CalHammingDist( imageL1Code , L1Code , 'vec' );
    
    for dist = 0 : L1Dis 
        % handle distance of dist starts
        vec = find( hmd == dist );
        
        for a = 1 : size( vec , 1 )
            
            fileName = [ 'E' num2str( vec(a) ) '.mat' ];
            load( fileName );
            
            Level2Search( imageGist , imageVector , imageLabel , trGist , trLabels , trVector , E2 , E3 );
            
        end
        
        % handle distance of dist ends
    end
    
    % We use the numOfRet and numOfHit vector to get the recall-precision
    % curve
    [ recall precision ] = processNumOfHitNumOfRet( numOfHit , numOfReturn );
    %
    
    [ rr , pp ] = avgRPPlot( recall , precision , 0.05 );    
    r = [ r , rr ];
    p = [ p , pp ];
    %search one image finishes
    
end