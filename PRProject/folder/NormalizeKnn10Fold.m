function [time2classifeOneSample] = NormalizeKnn10Fold ( folds )

load yeast.out

%normal the rawData
%map them to 0~1
rawData = yeast(:,1:8)';
rawData = mapminmax( rawData , 0 , 1);
rawData = rawData';

yeast = [ rawData yeast(:,9) ];


[M N ] = size( yeast );

%1~8 is attribute 9 is class label
% DS = dataset( yeast(:,1:8) , yeast(:,9));
% 
% s = size( DS.labels );
% s = s(1);
% 
% x = zeros( s , 1 );

indices = crossvalind( 'Kfold' , M , folds );

Xaccuracy = zeros( 15 , 1 );                %preallocate
Taccuracy = zeros( 15 , 1 );
tTTimeElasped = zeros( 15 , 1 );
tXTimeElasped = zeros( 15 , 1 );
tSTimeElasped = zeros( 15 , 2 );
time2classifeOneSample = zeros( 15 ,2 );

for k = 1 : 15
      
    for f = 1 : folds
        test = ( indices == f );
        train = ~test;
        TS = yeast( train , : );
        XS = yeast( test , : );
        TS = dataset( TS( : , 1:8 ) , TS(:,9) );
        XS = dataset( XS( : , 1:8 ) , XS(:,9) );
        
        tstart = tic;
        classifier = knnc( TS ,k );                  %train a k classifier with Training dataset
        tTTimeElasped(k) = tTTimeElasped(k) + toc( tstart );

        tstart = tic;
        Xoutput = labeld( XS , classifier);      %get the teXting result
        tXTimeElasped(k) = tXTimeElasped(k) + toc( tstart );
    
        len = size( Xoutput );
        len = len( 1 );
         j = 0;
         for i = 1:len
                if( Xoutput(i) == XS.labels(i) )
                    j = j + 1;
                end
         end
          
        Xaccuracy(k) = Xaccuracy(k) + j / len ;
 
         
        tstart = tic;
        Toutput = labeld( TS , classifier);      %get the Training resullt
        tSTimeElasped(k) = tSTimeElasped(k) + toc( tstart );
    
        len = size( Toutput );
        len = len(1);
        j = 0;
        for i = 1:len
            if( Toutput(i) == TS.labels(i) )
                j = j + 1;
            end
        end
           
        Taccuracy(k) = Taccuracy(k) + j * 1.0 / len;
    
    end
    
    tTTimeElasped(k) = tTTimeElasped(k) / folds;
    tXTimeElasped(k) = tXTimeElasped(k) / folds;
    time2classifeOneSample(k,1) = k;
    time2classifeOneSample(k,2) = tXTimeElasped(k) / ( M * 1 / folds);
    tSTimeElasped(k) = tSTimeElasped(k) / folds;
    Xaccuracy(k) = Xaccuracy(k) / folds;
    Taccuracy(k) = Taccuracy(k) / folds;
        
end

k = 1:1:15;

h = figure;
%title([num2str(precent_of_training * 100) '% of trainning' ]);
bigTitle = ['Cross-validation using ' num2str(folds) 'folds' ];
set(h,'name',bigTitle,'Numbertitle','off');

subplot(2,1,1),plot( k , Taccuracy , 'r-.+' , k , Xaccuracy , 'b-o' ),grid on ,ylabel('Accuracy'),xlabel('K'),title('Testing Accuracy of Precentage KNN classifier(Red is trainging acc Blue is testing acc');
subplot(2,1,2),plot( k , tTTimeElasped , 'r' , k , tXTimeElasped , 'g' , k , tSTimeElasped , 'b' ), grid on , xlabel('K') , ylabel('time elasped in sec') , title('Red is traing time; Green is label testing set time; Blue is training set time');


