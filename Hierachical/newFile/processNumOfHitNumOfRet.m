function [ recall precision ] = processNumOfHitNumOfRet( numOfHit , numOfRet )
% We have the numOfHit and numOfRet variabel
% numbers of return images [ 10 6 13 ... ]
% numbers of hit images    [ 10 2 5  ... ] 
% These indicate first return 10images and 10hit , second return 6 images
% and 2hit , third return 13images and 5 hit.
% 
% We use this information to calculate the recall-precision

% We know that numOfSam = 5000, and we use 0.05 interval,
% So 5000*0.05 = 250

interval = 250;

recall = 0;
precision = 1;

totalReturn = 0;
totalHit = 0;
index = 1;
length = size( numOfRet , 1 );
level = 1;

while index < length
    while ( index < length ) && ( totalReturn < interval*level )
        totalReturn = totalReturn + numOfRet( index );
        totalHit = totalHit + numOfHit( index );
        index = index + 1;
    end
    
    recall = [ recall totalHit/5000 ];
    precision = [ precision totalHit/totalReturn ];
    level = level + 1;
        
end

