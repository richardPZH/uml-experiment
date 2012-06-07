function [ tTTimeElasped time2classifeOneSample Xaccuracy Taccuracy] = DTwithIGwithNoPrune( precent_of_training )

load yeast.dis

%1~8 is attribute 9 is class label
t = [ 3 4 7];
DS = dataset( yeast(:, t ) , yeast(:,9));

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

    tStart = tic;
    classifier = TREEC( TS , 'infcrit'  );                  %train a tree classifier with Information Gain 
    tTTimeElasped = toc( tStart );
    
    tStart = tic;
    Xoutput = labeld( XS , classifier);      %get the teXting result
    tXTimeElasped = toc( tStart );
    time2classifeOneSample = tXTimeElasped / s;
    
    len = size( Xoutput );
    len = len( 1 );
    j = 0;
    for i = 1:len
        if( Xoutput(i) == XS.labels(i) )
            j = j + 1;
        end
    end
           
    Xaccuracy = j * 1.0 / len;
    
    tStart = tic;
    Toutput = labeld( TS , classifier);      %get the Training resullt
    tSTimeElasped = toc( tStart );
    
    len = size( Toutput );
    len = len(1);
    j = 0;
    for i = 1:len
        if( Toutput(i) == TS.labels(i) )
            j = j + 1;
        end
    end
           
    Taccuracy = j * 1.0 / len;

end