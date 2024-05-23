#!/usr/bin/env python3
#
# Copyright (c) 2019-2024 Key4hep-Project.
#
# This file is part of Key4hep.
# See https://key4hep.github.io/key4hep-doc/ for further info.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import sys
from Configurables import (
    LcioEvent,
    PodioInput,
    PodioOutput,
    MarlinProcessorWrapper,
    EDM4hep2LcioTool,
    Lcio2EDM4hepTool,
)


def create_reader(input_files, evtSvc):
    """Create the appropriate reader for the input files"""
    if input_files[0].endswith(".slcio"):
        if any(not f.endswith(".slcio") for f in input_files):
            print("All input files need to have the same format (LCIO)")
            sys.exit(1)

        read = LcioEvent()
        read.Files = input_files
    else:
        if any(not f.endswith(".root") for f in input_files):
            print("All input files need to have the same format (EDM4hep)")
            sys.exit(1)
        read = PodioInput("PodioInput")
        evtSvc.inputs = input_files

    return read


def attach_edm4hep2lcio_conversion(algList, read):
    """Attach the edm4hep to lcio conversion if necessary e.g. when using create_reader. Should only be run after algList is complete."""
    if not isinstance(read, PodioInput):
        # nothing to convert :)
        return

    # find first wrapper
    for alg in algList:
        if isinstance(alg, MarlinProcessorWrapper):
            break

    EDM4hep2LcioInput = EDM4hep2LcioTool("InputConversion")
    EDM4hep2LcioInput.convertAll = True
    # Adjust for the different naming conventions
    EDM4hep2LcioInput.collNameMapping = {"MCParticles": "MCParticle"}
    alg.EDM4hep2LcioTool = EDM4hep2LcioInput


def attach_lcio2edm4hep_conversion(algList):
    """Attach EDM4hep to LCIO conversion in all points in the algList in front
    of PodioOutput algorithms, such that the complete LCIO event content is
    converted to EDM4hep
    """
    # Find all the indices where a PodioOutput algorithm appears
    podout_idcs = [i for i, a in enumerate(algList) if isinstance(a, PodioOutput)]
    # For each of those check the preceeding algorithms to attach a conversion
    # (if none exists already)
    for idx in podout_idcs:
        prev_algs = algList[idx - 1 :: -1]
        for alg in prev_algs:
            if isinstance(alg, MarlinProcessorWrapper):
                if alg.Lcio2EDM4hepTool is not None:
                    lcio2edm4hep = EDM4hep2LcioTool(
                        f"OutConv_{algList[idx].name()}_{alg.name()}"
                    )
                    lcio2edm4hep.convertAll = True
                    # Adapt for the slightly different naming schemes
                    lcio2edm4hep.collNameMapping = {"MCParticle": "MCParticles"}
                    alg.Lcio2EDM4hepTool = lcio2edm4hep

                # Assume that the user knows what they want in this case and
                # don't try to do something fancy
                break
