function [time2classifeOneSample main85componentCount ] = nonNoralizeKnn ( precent_of_training )

load yeast.out

%1~8 is attribute 9 is class label
[coef,score,latent,t2] = princomp( yeast(:,1:8) );
latent=100*latent/sum(latent);
s = size( latent );
s = s(1);
totle = 0;
for main85componentCount = 1 : s 
    totle = totle + latent(main85componentCount);
    if( totle > 85 )
        break;
    end
end

t = 1 : main85componentCount;
DS = dataset( score(:, t ) , yeast(:,9) );

s = size( DS.labels );
s = s(1);

%disp( s );
%disp( l );

%training sample dataset TS
%testing  sample dataset XS
[ TS XS ] = gendat( DS , precent_of_training * s );
s = size( XS.labels );
s = s(1);

%disp( ts.data );
%disp( xs.data );

%disp( 'Disping full dataset' );
%figure( 1 );
%scatterd(DS);
%title( 'Full DataSet' );

%disp( 'Disping training dataset');
%figure( 2 );
%scatterd(TS);
%title( 'Training DataSet');

%disp( 'Disping testing set');
%figure( 3 );
%scatterd(XS);
%title('Testing DataSet');

Xaccuracy = zeros( 15 , 1 );                %preallocate
Taccuracy = zeros( 15 , 1 );
tTTimeElasped = zeros( 15 , 1 );
tXTimeElasped = zeros( 15 , 1 );
tSTimeElasped = zeros( 15 , 2 );
time2classifeOneSample = zeros( 15 ,1 );

for k=1:15
    tStart = tic;
    classifier = knnc( TS ,k );                  %train a k classifier with Training dataset
    tTTimeElasped(k) = toc( tStart );
    
    tStart = tic;
    Xoutput = labeld( XS , classifier);      %get the teXting result
    tXTimeElasped(k) = toc( tStart );
    time2classifeOneSample( k , 1 ) = k;
    time2classifeOneSample( k , 2 ) = tXTimeElasped(k) / s;
    
    len = size( Xoutput );
    len = len( 1 );
    j = 0;
    for i = 1:len
        if( Xoutput(i) == XS.labels(i) )
            j = j + 1;
        end
    end
           
    Xaccuracy(k) = j * 1.0 / len;
    
    tStart = tic;
    Toutput = labeld( TS , classifier);      %get the Training resullt
    tSTimeElasped(k) = toc( tStart );
    
    len = size( Toutput );
    len = len(1);
    j = 0;
    for i = 1:len
        if( Toutput(i) == TS.labels(i) )
            j = j + 1;
        end
    end
           
    Taccuracy(k) = j * 1.0 / len;
    
end

k = 1:1:15;

h = figure;
%title([num2str(precent_of_training * 100) '% of trainning' ]);
bigTitle = [num2str(precent_of_training * 100) '% of trainning' ];
set(h,'name',bigTitle,'Numbertitle','off');

subplot(2,1,1),plot( k , Taccuracy , 'r-.+' , k , Xaccuracy , 'b-o' ),grid on ,ylabel('Accuracy'),xlabel('K'),title('Testing Accuracy of Precentage KNN classifier(Red is trainging acc Blue is testing acc');
subplot(2,1,2),plot( k , tTTimeElasped , 'r' , k , tXTimeElasped , 'g' , k , tSTimeElasped , 'b' ), grid on , xlabel('K') , ylabel('time elasped in sec') , title('Red is traing time; Green is label testing set time; Blue is training set time');




end