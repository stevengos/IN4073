function [measures] = load_measures(path)

    data = dlmread(path, ' ');
    
    measures.time = data(:,1);
    
    measures.ae1 = data(:,2);
    measures.ae2 = data(:,3);
    measures.ae3 = data(:,4);
    measures.ae4 = data(:,5);
    
    measures.sax = data(:,6);
    measures.say = data(:,7);
    measures.saz = data(:,8);
    
    measures.sp = data(:,9);
    measures.sq = data(:,10);
    measures.sr = data(:,11);
    
    measures.fp = data(:,12);
    measures.fq = data(:,13);
    measures.fr = data(:,14);
end