import csv
count=0
with open("production_files/BOM-BikeUSBv2.csv",'r') as f:
    reader=csv.reader(f)
    with open("production_files/BOM-BikeUSBv2_lcsc.csv",'w') as fo:
        writer=csv.writer(fo)
        for r in reader:
            n=r[1].count(',')+1
            n*=3#how many should there be parts for?
            ro=[r[0],r[3],n]
            writer.writerow(ro)
            #print(r[1]+" -> "+str(n))
            print(ro)
            count += n

print()
print("Component Count: " + str(count))

