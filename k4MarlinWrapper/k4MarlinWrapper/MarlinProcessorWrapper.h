/*
 * Copyright (c) 2019-2024 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef K4MARLINWRAPPER_MARLINPROCESSORWRAPPER_H
#define K4MARLINWRAPPER_MARLINPROCESSORWRAPPER_H

#include <stack>
#include <string>

#include <Gaudi/Algorithm.h>
#include <GaudiKernel/IEventProcessor.h>
#include <GaudiKernel/IRndmEngine.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/ToolHandle.h>

#include <EVENT/LCEvent.h>
#include <EVENT/LCRunHeader.h>

#include <marlin/EventModifier.h>
#include <marlin/Exceptions.h>
#include <marlin/Global.h>
#include <marlin/ProcessorEventSeeder.h>
#include <marlin/ProcessorMgr.h>
#include <marlin/StringParameters.h>

#include <TSystem.h>

#include "k4MarlinWrapper/converters/IEDMConverter.h"

namespace marlin {
class Processor;
class StringParameters;
} // namespace marlin

class MarlinProcessorWrapper : public Gaudi::Algorithm {
public:
  explicit MarlinProcessorWrapper(const std::string& name, ISvcLocator* pSL);
  StatusCode execute(const EventContext&) const final;
  StatusCode finalize() final;
  StatusCode initialize() final;

private:
  std::string m_verbosity = "ERROR";
  mutable marlin::Processor* m_processor = nullptr;

  /// Load libraries specified by MARLIN_DLL environment variable
  StatusCode loadProcessorLibraries() const;

  /// Instantiate the Marlin processor and assign name and parameters
  StatusCode instantiateProcessor(std::shared_ptr<marlin::StringParameters>& parameters,
                                  Gaudi::Property<std::string>& processorTypeStr);

  /// Parse the parameters from the Property
  std::shared_ptr<marlin::StringParameters>
  parseParameters(const Gaudi::Property<std::map<std::string, std::vector<std::string>>>& parameters,
                  std::string& verbosity) const;

  /// ProcessorType: The Type of the MarlinProcessor to use
  Gaudi::Property<std::string> m_processorType{this, "ProcessorType", {}};
  Gaudi::Property<std::map<std::string, std::vector<std::string>>> m_parameters{this, "Parameters", {}};

  mutable ToolHandle<IEDMConverter> m_edm_conversionTool{"IEDMConverter/EDM4hep2Lcio", this};
  mutable ToolHandle<IEDMConverter> m_lcio_conversionTool{"IEDMConverter/Lcio2EDM4hep", this};

  static std::stack<marlin::Processor*>& ProcessorStack();

  bool isReEntrant() const final { return false; }
};

std::stack<marlin::Processor*>& MarlinProcessorWrapper::ProcessorStack() {
  static std::stack<marlin::Processor*> stack;
  return stack;
}

#endif
