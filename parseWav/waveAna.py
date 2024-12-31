#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import argparse
import os

class waveAna(object):
    # color could be expanded to full range of html color (with names!)
    _COLORS = ['b', 'r', 'g', 'c', 'm', 'y', 'k', 'DarkOrange']
    def __init__(self):
        self.fs = 96000.0
        self.nFigs = 0

    def fromFile(self, fname, nChannels, dtype='int16') :
        data = np.fromfile(fname, dtype)

        mt = np.reshape(data, (int(len(data)/nChannels), nChannels))

        self.waves = np.transpose(mt)

    def setFs(self, fs):
        self.fs = fs

    def plot(self, channels=None, holdon=False, xa='time'):
        if xa == 'index':
            x = np.arange(len(self.waves[0]))
        else:
            x = np.arange(0.0, len(self.waves[0])/self.fs, 1/self.fs)

        if channels is None:
            channels = range(0, len(self.waves))
        elif not isinstance(channels, list):
            channels = [channels]
        if holdon == False:
            print("---------------")
            print(channels)
            for c in channels:
                try:
                    plt.figure(self.nFigs)
                    self.nFigs += 1
                    plt.plot(x, self.waves[c])
                    plt.savefig("{}.png".format(self.nFigs))
                    if (xa == 'index'):
                        plt.xlabel('index [pts]')
                    else:
                        plt.xlabel('time [sec]')
                    plt.ylabel('Magnitude [raw]')
                    plt.title('Waveform of Channel {}'.format(c))
                except Exception as e:
                    continue
                
        else:
            print("**************************************")
            plt.hold(True)
            self.nFigs += 1
            title = 'Waveform of Channel'
            for i in range(0, len(channels)):
                title += ' {} '.format(channels[i])
                if i >= len(self.__class__._COLORS):
                    color = self.__class__._COLORS[0]
                else:
                    color = self.__class__._COLORS[i]
                plt.plot(x, self.waves[channels[i]], color,
                        label="Channel {}".format(channels[i]))
            if (xa == 'index'):
                plt.xlabel('index [pts]')
            else:
                plt.xlabel('time [sec]')
            plt.ylabel('Magnitude [raw]')
            plt.title(title)
            plt.legend()
            plt.hold(False)
            plt.savefig("12.png")

    def spectrogram(self, channel):
        f, t, Sxx = signal.spectrogram(self.waves[channel], self.fs,
            signal.get_window('blackman', 512),
            512, 256, 512, False, True, 'density', -1, 'psd')
        Sxx_log = np.zeros(shape=(len(Sxx), len(Sxx[0])))
        for col in range(0, len(Sxx)):
            Sxx_log[col] = np.log(Sxx[col])
        plt.figure(self.nFigs)
        self.nFigs = self.nFigs + 1
        plt.pcolormesh(t, f, Sxx_log, cmap='jet')
        plt.xlabel('Time [sec]')

    def showFigures(self):
        # plt.savefig("abc.png")
        plt.show()
        # self.nFigs = 0
    
    # def saveFigures(self):
    #     plt.savefig("a.png")

if __name__ == "__main__":

    filepath= os.getcwd()+"\\"
    files=os.listdir(filepath)
    for file in files:
        if '.DAT' in file:
            print(file)
            ana = waveAna()
            ana.fromFile(file, 1)
            ana.plot([0], holdon=False)
            ana.spectrogram(0)
            ana.showFigures()
            # ana.saveFigures()