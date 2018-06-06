#include <iostream>
#include "../../FuzzyRule.h"
#include "../../FuzzyComposition.h"
#include "../../Fuzzy.h"
#include "../../FuzzyRuleConsequent.h"
#include "../../FuzzyOutput.h"
#include "../../FuzzyInput.h"
#include "../../FuzzyIO.h"
#include "../../FuzzySet.h"
#include "../../FuzzyRuleAntecedent.h"

using namespace std;

int main(int argc, char *argv[]) {
    Fuzzy* fuzzy = new Fuzzy();

    // FuzzyInput
    FuzzyInput* distance_e = new FuzzyInput(1);

    FuzzySet* peq_e = new FuzzySet(10,20,35);
    distance_e->addFuzzySet(peq_e);
    FuzzySet* med_e = new FuzzySet(30,150,280);
    distance_e->addFuzzySet(med_e);
    FuzzySet* grand_e = new FuzzySet(270,300,320);
    distance_e->addFuzzySet(grand_e);
    
    fuzzy->addFuzzyInput(distance);

    // FuzzyInput
    FuzzyInput* distance_d = new FuzzyInput(2);

    FuzzySet* peq_d = new FuzzySet(10,20,35);
    distance_d->addFuzzySet(peq_d);
    FuzzySet* med_d = new FuzzySet(30,150,280);
    distance_d->addFuzzySet(med_d);
    FuzzySet* grand_d = new FuzzySet(270,300,320);
    distance_d->addFuzzySet(grand_d);


    fuzzy->addFuzzyInput(distance_d);

    // FuzzyOutput
    FuzzyOutput* vel_e = new FuzzyOutput(1);

    FuzzySet* baixa_e = new FuzzySet(0, 50, 70);
    vel_e->addFuzzySet(baixa_e);
    FuzzySet* media_e = new FuzzySet(60, 155, 185);
    vel_e->addFuzzySet(media_e);
    FuzzySet* alta_e = new FuzzySet(185, 200, 255);
    vel_e->addFuzzySet(alta_e);

    fuzzy->addFuzzyOutput(vel_e);

    //output2
    FuzzyOutput* vel_d = new FuzzyOutput(2);

    FuzzySet* baixa_d = new FuzzySet(0, 50, 70);
    vel_d->addFuzzySet(baixa_d);
    FuzzySet* media_d = new FuzzySet(60, 155, 185);
    vel_d->addFuzzySet(media_d);
    FuzzySet* alta_d = new FuzzySet(185, 200, 255);
    vel_d->addFuzzySet(alta_d);

    fuzzy->addFuzzyOutput(vel_d);
    ///TROCADO FUZZY ATÉ AQUI
    // Building FuzzyRule
    FuzzyRuleAntecedent* distanceCloseAndSpeedQuick = new FuzzyRuleAntecedent();
    distanceCloseAndSpeedQuick->joinWithAND(close, quick);
    FuzzyRuleAntecedent* temperatureCold = new FuzzyRuleAntecedent();
    temperatureCold->joinSingle(cold);
    FuzzyRuleAntecedent* ifDistanceCloseAndSpeedQuickOrTemperatureCold = new FuzzyRuleAntecedent();
    ifDistanceCloseAndSpeedQuickOrTemperatureCold->joinWithOR(distanceCloseAndSpeedQuick, temperatureCold);

    FuzzyRuleConsequent* thenRisMaximumAndSpeedSlow = new FuzzyRuleConsequent();
    thenRisMaximumAndSpeedSlow->addOutput(maximum);
    thenRisMaximumAndSpeedSlow->addOutput(slowOut);

    FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifDistanceCloseAndSpeedQuickOrTemperatureCold, thenRisMaximumAndSpeedSlow);
    fuzzy->addFuzzyRule(fuzzyRule1);

    // Building FuzzyRule
    FuzzyRuleAntecedent* distanceSafeAndSpeedNormal = new FuzzyRuleAntecedent();
    distanceSafeAndSpeedNormal->joinWithAND(safe, normal);
    FuzzyRuleAntecedent* ifDistanceSafeAndSpeedNormalOrTemperatureGood = new FuzzyRuleAntecedent();
    ifDistanceSafeAndSpeedNormalOrTemperatureGood->joinWithOR(distanceSafeAndSpeedNormal, good);

    FuzzyRuleConsequent* thenRiskAverageAndSpeedNormal = new FuzzyRuleConsequent();
    thenRiskAverageAndSpeedNormal->addOutput(average);
    thenRiskAverageAndSpeedNormal->addOutput(normalOut);

    FuzzyRule* fuzzyRule2 = new FuzzyRule(2, ifDistanceSafeAndSpeedNormalOrTemperatureGood, thenRiskAverageAndSpeedNormal);
    fuzzy->addFuzzyRule(fuzzyRule2);

    // Building FuzzyRule
    FuzzyRuleAntecedent* distanceDistanteAndSpeedSlow = new FuzzyRuleAntecedent();
    distanceDistanteAndSpeedSlow->joinWithAND(distante, slow);
    FuzzyRuleAntecedent* ifDistanceDistanteAndSpeedSlowOrTemperatureHot = new FuzzyRuleAntecedent();
    ifDistanceDistanteAndSpeedSlowOrTemperatureHot->joinWithOR(distanceDistanteAndSpeedSlow, hot);

    FuzzyRuleConsequent* thenRiskMinimumSpeedQuick = new FuzzyRuleConsequent();
    thenRiskMinimumSpeedQuick->addOutput(minimum);
    thenRiskMinimumSpeedQuick->addOutput(quickOut);

    FuzzyRule* fuzzyRule3 = new FuzzyRule(3, ifDistanceDistanteAndSpeedSlowOrTemperatureHot, thenRiskMinimumSpeedQuick);
    fuzzy->addFuzzyRule(fuzzyRule3);

    fuzzy->setInput(1, 10);
    fuzzy->setInput(2, 30);
    fuzzy->setInput(3, -15);

    fuzzy->fuzzify();

    cout << "Distância: " << close->getPertinence() << ", " << safe->getPertinence() << ", " << distante->getPertinence() << endl;
    cout << "Velocidade: " << stoped->getPertinence() << ", " << slow->getPertinence() << ", " << normal->getPertinence() << ", " << quick->getPertinence() << endl;
    cout << "Temperatura: " << cold->getPertinence() << ", " << good->getPertinence() << ", " << hot->getPertinence() << endl;

    cout << "regra1: " << fuzzyRule1->isFired() << ", regra2: " << fuzzyRule2->isFired() << ", regra3: " << fuzzyRule3->isFired() << endl;

    float output1 = fuzzy->defuzzify(1);
    float output2 = fuzzy->defuzzify(2);

    cout << "Saída Risco: " << output1 << ", Saída Velocidade: " << output2 << endl;
    
    return 0;
}