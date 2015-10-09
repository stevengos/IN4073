function [measures] = load_filters(path)

    data = dlmread(path, ' ');
    
    measures.time = data(:,1);
    
    measures.sax = data(:,2);
    measures.say = data(:,3);
    measures.saz = data(:,4);
    
    measures.sp = data(:,5);
    measures.sq = data(:,6);
    measures.sr = data(:,7);
    
    measures.fax = data(:,8);
    measures.fay = data(:,9);
    measures.faz = data(:,10);
    
    measures.fp = data(:,11);
    measures.fq = data(:,12);
    measures.fr = data(:,13);
end