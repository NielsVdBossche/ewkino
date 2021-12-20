import json
from ROOT import TH1D, TFile
from array import array

def checkConsistentWidth(binning):
    # function to check if a constant width is used in a binning
    width = binning[1] - binning[0]
    for i in range(1, len(binning)):
        newWidth = binning[i] - binning[i-1]
        if (width != newWidth):
            return False

    return True

def generateHistogram(name, binning):
    if (checkConsistentWidth(binning)):
        histogram = TH1D(name, name, len(binning) - 1, binning[0], binning[-1])
    else:
        binArray = array('d', binning[:-1])
        print(binArray)
        
        histogram = TH1D(name, name, len(binning) - 1, binArray)

    return histogram

def generateHistogramVariation(jsonContent):
    name = jsonContent["key"]
    binning = jsonContent["value"]["edges"]
    content = jsonContent["value"]["content"]

    hist = generateHistogram(name, binning)
    for i, val in enumerate(content):
        hist.SetBinContent(i+1, val)
    
    return hist

def generateAllHistogramVariations(outfileName, jsonContent):
    print(len(jsonContent))
    file = TFile("/home/njovdnbo/Documents/ewkino/weights/JSONCode/weightFiles/" + outfileName + ".root", "recreate")
    for i in range(len(jsonContent)):
        hist = generateHistogramVariation(jsonContent[i])
        hist.Write(hist.GetName())

if __name__ == "__main__":
    # do stuff
    names = ["PU_2016PostVFP.json",  "PU_2016PreVFP.json",  "PU_2017.json",  "PU_2018.json"]
    for filename in names:
        f = open("/home/njovdnbo/Documents/ewkino/weights/JSONCode/weightFiles/" + filename)

        data = json.load(f)
        
        # following path leads to the interesting stuff
        #print(data["corrections"][0]["data"]["content"])
        usefulData = data["corrections"][0]["data"]["content"]
        jsonID = data["corrections"][0]["name"]
        #next up: generate histogram using the binning provided in the binning node and content set to the stuff in nominal
        generateAllHistogramVariations(jsonID, usefulData)
