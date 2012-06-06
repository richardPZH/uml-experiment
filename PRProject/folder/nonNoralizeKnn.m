function [] = lab2e1 ( precent_of_training )

load yeast.out

%1~8 is attribute 9 is class label
DS = dataset( yeast(:,1:8) , yeast(:,9));

s = size( DS.labels );
s = s(1);

%disp( s );
%disp( l );

%training sample dataset TS
%testing  sample dataset XS
[ TS XS ] = gendat( DS , precent_of_training * s );

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

for k=1:15
    classifier = knnc( TS ,k );                  %train a k classifier with Training dataset
    
    Xoutput = labeld( XS , classifier);      %get the teXting result
    len = size( Xoutput );
    len = len( 1 );
    j = 0;
    for i = 1:len
        if( Xoutput(i) == XS.labels(i) )
            j = j + 1;
        end
    end
           
    Xaccuracy(k) = j * 1.0 / len;
    
    Toutput = labeld( TS , classifier);      %get the Training resullt
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
plot( k , Taccuracy , 'r-.+');
hold on;
plot( k , Xaccuracy , 'b-o');

grid on;
ylabel('Accuracy');
xlabel('K');
title('Testing Accuracy of Precentage KNN classifier(Red is trainging acc Blue is testing acc');

end