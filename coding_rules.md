# Règle de messages de commit

But : s'assurer que tous les commits respectent une convention simple pour faciliter la lecture et l'automatisation.

Règle :
- Le message de commit doit commencer par l'un des tags suivants : `[DEV]`, `[DOC]`, `[MINOR]`.

Expression régulière (match au début du message) :

Exemples valides :
- `[DEV] Correction du capteur de température`
- `[DOC] Mise à jour du README`
- `[MINOR] Correction typo`

Exemples invalides :
- `Fix bug dans la lecture ADC`
- `Updated README`

Application automatique :
- Un workflow GitHub Actions est fourni : `.github/workflows/commit_rules.yml`.  
  - Activez la protection de branche (Settings → Branches → Branch protection rules) pour la branche cible (ex. `main`) et exigez que le check du workflow passe avant merge.
- Optionnel (contrôle local) : ajouter un hook `commit-msg` versionné (ex. `.githooks/commit-msg`) et configurer `git config core.hooksPath .githooks`.

Notes :
- Adapter la regex si vous voulez autoriser le tag ailleurs que le début du message.
- Pour les PRs depuis des forks publics, le workflow peut toujours vérifier les messages ; certains checks utilisant des secrets ne s'exécuteront pas.