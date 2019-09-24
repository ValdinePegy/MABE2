/**
 *  @note This file is part of MABE, https://github.com/mercere99/MABE2
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2019
 *
 *  @file  SelectElite.h
 *  @brief MABE module to enable elite selection.
 */

#ifndef MABE_SELECT_ELITE_H
#define MABE_SELECT_ELITE_H

#include "../core/MABE.h"
#include "../core/Module.h"

#include "tools/valsort_map.h"

namespace mabe {

  /// Add elite selection with the current population.
  class SelectElite : public Module {
  private:
    std::string trait;    ///< Which trait should we select on?
    size_t top_count=1;   ///< Top how-many should we select?
    size_t copy_count=1;  ///< How many copies of each should we make?
    int pop_id = 0;       ///< Which population are we selecting from?

  public:
    SelectElite(mabe::MABE & control,
               const std::string & name="SelectElite",
               const std::string & desc="Module to choose the top fitness organisms for replication.",
               const std::string & in_trait="fitness", size_t tcount=1, size_t ccount=1)
      : Module(control, name, desc)
      , trait(in_trait), top_count(tcount), copy_count(ccount)
    {
      SetSelectMod(true);               ///< Mark this module as a selection module.
      AddRequiredTrait<double>(trait);  ///< The fitness trait must be set by another module.
      SetMinPops(1);                    ///< Must run elite selection on a population.
    } 
    ~SelectElite() { }

    void SetupConfig() override {
      LinkPop(pop_id, "target_pop", "Which population should we select parents from?");
      LinkVar(top_count, "top_count", "Number of top-fitness orgs to be replicated", 1);
      LinkVar(copy_count, "copy_count", "Number of copies to make of replicated organisms", 1);
      LinkVar(trait, "fitness_trait", "Which trait provides the fitness value to use?", "fitness");
    }

    void SetupModule() override { }

    void OnUpdate(size_t update) override {
      // Construct a map of all IDs to their associated fitness values.
      emp::valsort_map<OrgPosition, double> id_fit_map;
      Population & pop = control.GetPopulation(pop_id);
      for (auto it = pop.begin_alive(); it != pop.end_alive(); it++) {
        id_fit_map.Set(it, it->GetVar<double>(trait));
        //std::cout << "Measuring fit " << it->GetVar<double>(trait) << std::endl;
      }

      // Loop through the IDs in fitness order (from highest), replicating each
      size_t num_reps = 0;
      for (auto it = id_fit_map.crvbegin(); it != id_fit_map.crvend() && num_reps++ < top_count; it++) {
        //std::cout << "Replicating fit " << it->first->GetVar<double>(trait) << std::endl;
        control.Replicate(it->first, copy_count);
      }
    }
  };

}

#endif