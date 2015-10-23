function [measures] = load_measures(path)

    data = dlmread(path, ' ');
    
    measures.time = data(:,1);
    
    measures.battery = data(:,2);

    measures.ae1 = data(:,3);
    measures.ae2 = data(:,4);
    measures.ae3 = data(:,5);
    measures.ae4 = data(:,6);
    
    measures.sax = data(:,7);
    measures.say = data(:,8);
    measures.saz = data(:,9);
    
    measures.sp = data(:,10);
    measures.sq = data(:,11);
    measures.sr = data(:,12);

    measures.fax = data(:,13);
    measures.fay = data(:,14);
    measures.faz = data(:,15);
    
    measures.fp = data(:,16);
    measures.fq = data(:,17);
    measures.fr = data(:,18);
end
