#!../../bin/windows-x64-mingw/sampleIOC

#- SPDX-FileCopyrightText: 2000 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change sampleIOC to something else
#- everywhere it appears in this file

< envPaths

epicsEnvSet ("STREAM_PROTOCOL_PATH","$(TOP)/db")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/sampleIOC.dbd"
sampleIOC_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/sampleIOCVersion.db", "user=LeeKarter"
dbLoadRecords "db/dbSubExample.db", "user=LeeKarter"

drvAsynIPPortConfigure("L0","127.0.0.1:9999",0,0,0)

dbLoadRecords("db/MDO.db", "P=LeeKarter:,PORT=L0,R=MDO:,L=0,A=0")

#- Set this to see messages from mySub
#-var mySubDebug 1

#- Run this to trace the stages of iocInit
#- traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=LeeKarter"
